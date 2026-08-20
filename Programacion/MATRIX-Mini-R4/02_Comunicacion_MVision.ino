#pragma once

#include <Arduino.h>
#include <MatrixMiniR4.h>

/*
  ============================================================
  CAMARA_MVISION.h
  Comunicacion UART M-Vision -> MATRIX Mini R4
  ============================================================

  La camara enviara exactamente cuatro valores:

    [estado, objeto_id, confianza, evento]

  estado     0 = sin hallazgo confirmado / 1 = hallazgo confirmado
  objeto_id  1 = SELLO / 2 = PUNTA / 3..5 reservados
  confianza  0 a 100
  evento     numero que cambia solo ante un hallazgo nuevo

  La libreria oficial MATRIX usa el UART fisico Serial1 mediante:

    MiniR4.Vision.Begin();
    MiniR4.Vision.SmartCamReader(datos, timeout);

  Este modulo NO decide como detectar colores o formas. Esa inteligencia se
  programara despues dentro de la M-Vision con OpenMV/MicroPython.
*/

struct DatosMVision
{
    bool valido = false;
    bool detectado = false;
    uint8_t objetoId = 0;
    uint8_t confianza = 0;
    uint32_t eventoCamara = 0;
    uint32_t recibidoMs = 0;
};


class CamaraMVision
{
public:
    void comenzar(bool diagnosticoSerial = true)
    {
        diagnosticoSerial_ = diagnosticoSerial;
        MiniR4.Vision.Begin();
        iniciada_ = true;
        ultimaRecepcionValidaMs_ = 0;
        ultimoEventoDetectado_ = 0;
        hallazgoPendiente_ = false;
        ultimoCodigoLectura_ = 0;

        if (diagnosticoSerial_)
        {
            Serial.println("[M-VISION] UART iniciado a 115200");
            Serial.println("[M-VISION] Esperando [estado,objeto,confianza,evento]");
        }
    }


    // Llamar en cada vuelta de loop. La lectura usa un timeout corto para no
    // detener motores, sensores ni la comunicacion WiFi del robot.
    void actualizar()
    {
        if (!iniciada_)
        {
            return;
        }

        if (millis() - ultimoIntentoLecturaMs_ < INTERVALO_LECTURA_MS)
        {
            return;
        }

        ultimoIntentoLecturaMs_ = millis();
        bool estabaConectada = conectada();

        ultimoCodigoLectura_ = MiniR4.Vision.SmartCamReader(
            datosCrudos_,
            TIMEOUT_LECTURA_MS
        );

        // -1 significa simplemente que no llego un paquete en esta vuelta.
        if (ultimoCodigoLectura_ < 0)
        {
            if (
                ultimoCodigoLectura_ != -1 &&
                diagnosticoSerial_ &&
                millis() - ultimoErrorMostradoMs_ >= 1200
            )
            {
                ultimoErrorMostradoMs_ = millis();
                Serial.print("[M-VISION] Paquete UART invalido: ");
                Serial.println(ultimoCodigoLectura_);
            }

            return;
        }

        if (ultimoCodigoLectura_ < 4)
        {
            paquetesInvalidos_++;
            return;
        }

        uint32_t estado = datosCrudos_[0];
        uint32_t objeto = datosCrudos_[1];
        uint32_t confianza = datosCrudos_[2];
        uint32_t evento = datosCrudos_[3];

        if (
            estado > 1 ||
            objeto > 5 ||
            confianza > 100
        )
        {
            paquetesInvalidos_++;

            if (diagnosticoSerial_)
            {
                Serial.println("[M-VISION] Datos fuera de rango ignorados");
            }

            return;
        }

        ultimaRecepcionValidaMs_ = millis();
        paquetesValidos_++;

        // Si la camara se acaba de reconectar, permitimos que su contador de
        // eventos haya comenzado nuevamente desde 1.
        if (!estabaConectada)
        {
            ultimoEventoDetectado_ = 0;

            if (diagnosticoSerial_)
            {
                Serial.println("[M-VISION] Camara comunicando correctamente");
            }
        }

        ultimoDato_.valido = true;
        ultimoDato_.detectado = estado == 1;
        ultimoDato_.objetoId = (uint8_t)objeto;
        ultimoDato_.confianza = (uint8_t)confianza;
        ultimoDato_.eventoCamara = evento;
        ultimoDato_.recibidoMs = millis();

        bool hallazgoValido =
            estado == 1 &&
            objeto >= 1 &&
            objeto <= 5 &&
            evento != 0;

        if (hallazgoValido && evento != ultimoEventoDetectado_)
        {
            ultimoEventoDetectado_ = evento;
            hallazgoPendiente_ = true;
            hallazgo_ = ultimoDato_;

            if (diagnosticoSerial_)
            {
                Serial.print("[M-VISION] Hallazgo nuevo: ");
                Serial.print(nombreObjeto((uint8_t)objeto));
                Serial.print(" · ");
                Serial.print(confianza);
                Serial.print("% · evento camara ");
                Serial.println(evento);
            }
        }
    }


    bool conectada() const
    {
        return
            ultimaRecepcionValidaMs_ != 0 &&
            millis() - ultimaRecepcionValidaMs_ < TIMEOUT_CAMARA_OFFLINE_MS;
    }


    bool hayNuevoHallazgo() const
    {
        return hallazgoPendiente_;
    }


    DatosMVision consumirHallazgo()
    {
        DatosMVision resultado = hallazgo_;
        hallazgoPendiente_ = false;
        return resultado;
    }


    const DatosMVision& ultimoDato() const
    {
        return ultimoDato_;
    }


    int ultimoCodigoLectura() const
    {
        return ultimoCodigoLectura_;
    }


    uint32_t paquetesValidos() const
    {
        return paquetesValidos_;
    }


    uint32_t paquetesInvalidos() const
    {
        return paquetesInvalidos_;
    }


    static const char* nombreObjeto(uint8_t id)
    {
        switch (id)
        {
            case 1:
                return "SELLO";

            case 2:
                return "PUNTA";

            case 3:
                return "MOAI";

            case 4:
                return "CABALLO";

            case 5:
                return "HNEFATAFL";

            default:
                return "NINGUNO";
        }
    }

private:
    enum : uint32_t
    {
        INTERVALO_LECTURA_MS = 15,
        TIMEOUT_LECTURA_MS = 10,
        TIMEOUT_CAMARA_OFFLINE_MS = 2500
    };

    bool diagnosticoSerial_ = true;
    bool iniciada_ = false;
    bool hallazgoPendiente_ = false;

    unsigned int datosCrudos_[10] = {0};

    DatosMVision ultimoDato_;
    DatosMVision hallazgo_;

    int ultimoCodigoLectura_ = 0;
    uint32_t ultimoIntentoLecturaMs_ = 0;
    uint32_t ultimaRecepcionValidaMs_ = 0;
    uint32_t ultimoEventoDetectado_ = 0;
    uint32_t ultimoErrorMostradoMs_ = 0;
    uint32_t paquetesValidos_ = 0;
    uint32_t paquetesInvalidos_ = 0;
};

