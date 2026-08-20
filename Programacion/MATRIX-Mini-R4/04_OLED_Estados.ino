#pragma once

#include <Arduino.h>
#include <MatrixMiniR4.h>

/*
  ============================================================
  PANTALLA_ESTADOS.h
  OLED, RGB, buzzer, animaciones y estados de ATOM-ArchBot
  ============================================================

  Esta pestana concentra toda la presentacion del sistema. El programa del
  robot solo informa el estado; no necesita repetir codigo de OLED o luces.
*/

enum EstadoSistema
{
    INICIALIZANDO,
    LISTO,
    HARNIENDO,
    PAUSADO,
    HALLAZGO,
    FINALIZADO,
    ERROR_SISTEMA
};


class PantallaEstados
{
public:
    void comenzar()
    {
        MiniR4.LED.setBrightness(1, 80);
        MiniR4.LED.setBrightness(2, 80);

        estado_ = INICIALIZANDO;
        vistaTemporal_ = VISTA_NINGUNA;
        ultimoFrameMs_ = 0;
        frame_ = 0;
        forzarRedibujo_ = true;
        aplicarRGB();
        renderizar();
    }


    void cambiarEstado(EstadoSistema nuevoEstado)
    {
        if (estado_ == nuevoEstado)
        {
            return;
        }

        EstadoSistema anterior = estado_;
        estado_ = nuevoEstado;
        vistaTemporal_ = VISTA_NINGUNA;
        forzarRedibujo_ = true;

        if (nuevoEstado == LISTO && anterior == INICIALIZANDO)
        {
            sonidoEncendido();
        }
        else if (nuevoEstado == PAUSADO)
        {
            sonidoPausa();
        }
        else if (nuevoEstado == HARNIENDO && anterior == PAUSADO)
        {
            sonidoReanudar();
        }
        else if (nuevoEstado == HALLAZGO)
        {
            sonidoHallazgo();
        }
        else if (nuevoEstado == ERROR_SISTEMA)
        {
            sonidoError();
        }

        aplicarRGB();
    }


    EstadoSistema estado() const
    {
        return estado_;
    }


    void actualizarConexiones(
        bool wifiConectado,
        bool atomIAOnline,
        bool camaraConectada
    )
    {
        if (
            wifiConectado_ != wifiConectado ||
            atomIAOnline_ != atomIAOnline ||
            camaraConectada_ != camaraConectada
        )
        {
            wifiConectado_ = wifiConectado;
            atomIAOnline_ = atomIAOnline;
            camaraConectada_ = camaraConectada;
            forzarRedibujo_ = true;
        }
    }


    void actualizarDatosCamara(
        uint8_t objetoId,
        uint8_t confianza,
        uint32_t eventoCamara
    )
    {
        objetoId_ = objetoId;
        confianza_ = confianza;
        eventoCamara_ = eventoCamara;
        forzarRedibujo_ = true;
    }


    void mostrarEnvioATOMIA()
    {
        vistaTemporal_ = VISTA_ENVIANDO;
        inicioVistaTemporalMs_ = millis();
        forzarRedibujo_ = true;
        rgbAzul();
    }


    void mostrarResultadoEnvio(
        bool correcto,
        uint32_t eventoATOMIA,
        int codigoHTTP
    )
    {
        eventoATOMIA_ = eventoATOMIA;
        codigoHTTP_ = codigoHTTP;
        vistaTemporal_ = correcto ? VISTA_ENVIADO_OK : VISTA_ENVIO_ERROR;
        inicioVistaTemporalMs_ = millis();
        forzarRedibujo_ = true;

        if (correcto)
        {
            rgbVerde();
            sonidoEnvioCorrecto();
        }
        else
        {
            rgbRojo();
            sonidoError();
        }
    }


    void actualizar()
    {
        if (
            (vistaTemporal_ == VISTA_ENVIADO_OK ||
             vistaTemporal_ == VISTA_ENVIO_ERROR) &&
            millis() - inicioVistaTemporalMs_ >= DURACION_RESULTADO_MS
        )
        {
            vistaTemporal_ = VISTA_NINGUNA;
            forzarRedibujo_ = true;
        }

        if (
            !forzarRedibujo_ &&
            millis() - ultimoFrameMs_ < INTERVALO_ANIMACION_MS
        )
        {
            return;
        }

        ultimoFrameMs_ = millis();
        frame_++;
        aplicarRGB();
        renderizar();
        forzarRedibujo_ = false;
    }


    static const char* nombreEstado(EstadoSistema estado)
    {
        switch (estado)
        {
            case INICIALIZANDO:
                return "INICIALIZANDO";

            case LISTO:
                return "LISTO";

            case HARNIENDO:
                return "HARNIENDO";

            case PAUSADO:
                return "PAUSADO";

            case HALLAZGO:
                return "HALLAZGO";

            case FINALIZADO:
                return "FINALIZADO";

            default:
                return "ERROR";
        }
    }

private:
    enum VistaTemporal
    {
        VISTA_NINGUNA,
        VISTA_ENVIANDO,
        VISTA_ENVIADO_OK,
        VISTA_ENVIO_ERROR
    };

    enum : uint32_t
    {
        INTERVALO_ANIMACION_MS = 180,
        DURACION_RESULTADO_MS = 1800
    };

    EstadoSistema estado_ = INICIALIZANDO;
    VistaTemporal vistaTemporal_ = VISTA_NINGUNA;

    bool wifiConectado_ = false;
    bool atomIAOnline_ = false;
    bool camaraConectada_ = false;
    bool forzarRedibujo_ = true;

    uint8_t objetoId_ = 0;
    uint8_t confianza_ = 0;
    uint32_t eventoCamara_ = 0;
    uint32_t eventoATOMIA_ = 0;
    int codigoHTTP_ = 0;

    uint8_t frame_ = 0;
    uint32_t ultimoFrameMs_ = 0;
    uint32_t inicioVistaTemporalMs_ = 0;


    void renderizar()
    {
        MiniR4.OLED.clearDisplay();
        MiniR4.OLED.setTextSize(1);

        if (vistaTemporal_ == VISTA_ENVIANDO)
        {
            dibujarEnvio();
        }
        else if (vistaTemporal_ == VISTA_ENVIADO_OK)
        {
            dibujarResultado(true);
        }
        else if (vistaTemporal_ == VISTA_ENVIO_ERROR)
        {
            dibujarResultado(false);
        }
        else
        {
            dibujarEstado();
        }

        MiniR4.OLED.display();
    }


    void dibujarEstado()
    {
        MiniR4.OLED.setCursor(0, 0);
        MiniR4.OLED.print(nombreEstado(estado_));
        MiniR4.OLED.print(" ");
        MiniR4.OLED.print(simboloAnimacion());

        MiniR4.OLED.setCursor(0, 8);
        MiniR4.OLED.print("CAM:");
        MiniR4.OLED.print(camaraConectada_ ? "OK" : "--");
        MiniR4.OLED.print(" IA:");
        MiniR4.OLED.print(atomIAOnline_ ? "OK" : (wifiConectado_ ? ".." : "--"));

        MiniR4.OLED.setCursor(0, 16);

        if (estado_ == INICIALIZANDO)
        {
            MiniR4.OLED.print("Revisando sistemas");
        }
        else if (estado_ == LISTO)
        {
            MiniR4.OLED.print("UP para comenzar");
        }
        else if (estado_ == HARNIENDO)
        {
            MiniR4.OLED.print("Buscando objeto...");
        }
        else if (estado_ == PAUSADO)
        {
            MiniR4.OLED.print("UP para reanudar");
        }
        else if (estado_ == HALLAZGO)
        {
            MiniR4.OLED.print("OBJ:");
            MiniR4.OLED.print(nombreObjeto(objetoId_));
            MiniR4.OLED.print(" ");
            MiniR4.OLED.print(confianza_);
            MiniR4.OLED.print("%");
        }
        else if (estado_ == FINALIZADO)
        {
            MiniR4.OLED.print("Proceso completo");
        }
        else
        {
            MiniR4.OLED.print("Revisar sistema");
        }

        MiniR4.OLED.setCursor(0, 24);

        if (estado_ == HALLAZGO)
        {
            MiniR4.OLED.print("MV-E:");
            MiniR4.OLED.print(eventoCamara_);
            MiniR4.OLED.print(" DN:seguir");
        }
        else
        {
            MiniR4.OLED.print("ATOM-ArchBot V3");
        }
    }


    void dibujarEnvio()
    {
        MiniR4.OLED.setCursor(0, 0);
        MiniR4.OLED.print("ENVIANDO A ATOM.IA");
        MiniR4.OLED.setCursor(0, 10);
        MiniR4.OLED.print("OBJ:");
        MiniR4.OLED.print(nombreObjeto(objetoId_));
        MiniR4.OLED.print(" ");
        MiniR4.OLED.print(confianza_);
        MiniR4.OLED.print("%");
        MiniR4.OLED.setCursor(0, 22);
        MiniR4.OLED.print("Espere ");
        MiniR4.OLED.print(simboloAnimacion());
    }


    void dibujarResultado(bool correcto)
    {
        MiniR4.OLED.setCursor(0, 0);
        MiniR4.OLED.print(correcto ? "RECIBIDO POR IA" : "ERROR DE ENVIO");
        MiniR4.OLED.setCursor(0, 10);

        if (correcto)
        {
            MiniR4.OLED.print("EVENTO: ");
            MiniR4.OLED.print(eventoATOMIA_);
        }
        else
        {
            MiniR4.OLED.print("HTTP: ");
            MiniR4.OLED.print(codigoHTTP_);
        }

        MiniR4.OLED.setCursor(0, 22);
        MiniR4.OLED.print(correcto ? "ENVIADO OK" : "ATOM IA reintenta");
    }


    char simboloAnimacion() const
    {
        const char animacion[4] = {'|', '/', '-', '\\'};
        return animacion[frame_ % 4];
    }


    const char* nombreObjeto(uint8_t id) const
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


    void aplicarRGB()
    {
        if (vistaTemporal_ == VISTA_ENVIANDO)
        {
            rgbAzul();
            return;
        }

        if (vistaTemporal_ == VISTA_ENVIADO_OK)
        {
            rgbVerde();
            return;
        }

        if (vistaTemporal_ == VISTA_ENVIO_ERROR)
        {
            if (frame_ % 2 == 0)
            {
                rgbRojo();
            }
            else
            {
                rgbApagado();
            }

            return;
        }

        switch (estado_)
        {
            case INICIALIZANDO:
                if (frame_ % 2 == 0)
                {
                    MiniR4.LED.setColor(1, 0, 80, 255);
                    MiniR4.LED.setColor(2, 0, 15, 60);
                }
                else
                {
                    MiniR4.LED.setColor(1, 0, 15, 60);
                    MiniR4.LED.setColor(2, 0, 80, 255);
                }
                break;

            case LISTO:
                rgbBlanco();
                break;

            case HARNIENDO:
                if (frame_ % 2 == 0)
                {
                    MiniR4.LED.setColor(1, 255, 0, 0);
                    MiniR4.LED.setColor(2, 90, 0, 0);
                }
                else
                {
                    MiniR4.LED.setColor(1, 90, 0, 0);
                    MiniR4.LED.setColor(2, 255, 0, 0);
                }
                break;

            case PAUSADO:
                rgbNaranja();
                break;

            case HALLAZGO:
                if (frame_ % 2 == 0)
                {
                    rgbVerde();
                }
                else
                {
                    rgbBlanco();
                }
                break;

            case FINALIZADO:
                rgbVerde();
                break;

            default:
                if (frame_ % 2 == 0)
                {
                    rgbRojo();
                }
                else
                {
                    rgbApagado();
                }
                break;
        }
    }


    void rgbRojo()
    {
        MiniR4.LED.setColor(1, 255, 0, 0);
        MiniR4.LED.setColor(2, 255, 0, 0);
    }


    void rgbVerde()
    {
        MiniR4.LED.setColor(1, 0, 255, 0);
        MiniR4.LED.setColor(2, 0, 255, 0);
    }


    void rgbAzul()
    {
        MiniR4.LED.setColor(1, 0, 80, 255);
        MiniR4.LED.setColor(2, 0, 80, 255);
    }


    void rgbNaranja()
    {
        MiniR4.LED.setColor(1, 255, 85, 0);
        MiniR4.LED.setColor(2, 255, 85, 0);
    }


    void rgbBlanco()
    {
        MiniR4.LED.setColor(1, 180, 180, 180);
        MiniR4.LED.setColor(2, 180, 180, 180);
    }


    void rgbApagado()
    {
        MiniR4.LED.setColor(1, 0, 0, 0);
        MiniR4.LED.setColor(2, 0, 0, 0);
    }


    void sonidoEncendido()
    {
        MiniR4.Buzzer.Tone(NOTE_C4, 100);
        delay(115);
        MiniR4.Buzzer.Tone(NOTE_E4, 100);
        delay(115);
        MiniR4.Buzzer.Tone(NOTE_G4, 130);
        delay(145);
        MiniR4.Buzzer.Tone(NOTE_C5, 280);
        delay(295);
        MiniR4.Buzzer.NoTone();
    }


    void sonidoHallazgo()
    {
        MiniR4.Buzzer.Tone(NOTE_G4, 80);
        delay(95);
        MiniR4.Buzzer.Tone(NOTE_C5, 100);
        delay(115);
        MiniR4.Buzzer.Tone(NOTE_E5, 100);
        delay(115);
        MiniR4.Buzzer.Tone(NOTE_G5, 250);
        delay(265);
        MiniR4.Buzzer.NoTone();
    }


    void sonidoPausa()
    {
        MiniR4.Buzzer.Tone(NOTE_G4, 70);
        delay(85);
        MiniR4.Buzzer.Tone(NOTE_E4, 70);
        delay(85);
        MiniR4.Buzzer.Tone(NOTE_D4, 70);
        delay(85);
        MiniR4.Buzzer.Tone(NOTE_C4, 120);
        delay(135);
        MiniR4.Buzzer.NoTone();
    }


    void sonidoReanudar()
    {
        MiniR4.Buzzer.Tone(NOTE_C4, 70);
        delay(85);
        MiniR4.Buzzer.Tone(NOTE_D4, 70);
        delay(85);
        MiniR4.Buzzer.Tone(NOTE_E4, 70);
        delay(85);
        MiniR4.Buzzer.Tone(NOTE_G4, 120);
        delay(135);
        MiniR4.Buzzer.NoTone();
    }


    void sonidoEnvioCorrecto()
    {
        MiniR4.Buzzer.Tone(NOTE_C5, 65);
        delay(80);
        MiniR4.Buzzer.Tone(NOTE_E5, 65);
        delay(80);
        MiniR4.Buzzer.Tone(NOTE_G5, 120);
        delay(135);
        MiniR4.Buzzer.NoTone();
    }


    void sonidoError()
    {
        MiniR4.Buzzer.Tone(NOTE_C4, 90);
        delay(105);
        MiniR4.Buzzer.Tone(NOTE_G3, 130);
        delay(145);
        MiniR4.Buzzer.NoTone();
    }
};

