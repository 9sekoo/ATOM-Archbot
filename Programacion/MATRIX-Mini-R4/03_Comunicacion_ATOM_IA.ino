#pragma once

#include <Arduino.h>
#include <WiFiS3.h>

/*
  ============================================================
  COMUNICACION_ATOM_IA.h
  Comunicacion MATRIX Mini R4 -> ESP32 -> ATOM.ia
  ============================================================

  Esta pestana se ocupa solamente de:
    - conectar MATRIX a ATOM-ARCHBOT;
    - mantener el heartbeat con la ESP32;
    - reconectar el WiFi automaticamente;
    - enviar hallazgos con el formato correcto;
    - repetir un envio sin duplicar el evento;
    - informar el estado al programa principal.

  No controla motores, servos, camara, botones, OLED, RGB ni buzzer.

  Uso minimo desde el programa principal:

    AtomIAComunicacion AtomIA;

    void setup() {
      AtomIA.comenzar();
    }

    void loop() {
      AtomIA.actualizar();

      if (hayHallazgoNuevo) {
        AtomIA.enviarHallazgo(objetoId, confianza);
      }
    }
*/

class AtomIAComunicacion
{
public:
    enum Estado
    {
        SIN_WIFI,
        ESPERANDO_ATOM_IA,
        ATOM_IA_ONLINE
    };

    AtomIAComunicacion() : esp32IP_(192, 168, 4, 1)
    {
    }

    // Inicia la conexion sin detener durante varios segundos al robot.
    // actualizar() se encargara de completar y mantener la conexion.
    void comenzar(bool diagnosticoSerial = true)
    {
        diagnosticoSerial_ = diagnosticoSerial;
        wifiConectadoAnterior_ = false;
        servidorOnline_ = false;
        fallosHeartbeat_ = 0;
        ultimoCodigoHTTP_ = 0;
        cambioEstado_ = true;

        if (diagnosticoSerial_)
        {
            Serial.println();
            Serial.println("[ATOM.IA] Iniciando comunicacion");
            Serial.println("[ATOM.IA] Red: ATOM-ARCHBOT");
            Serial.println("[ATOM.IA] Servidor: 192.168.4.1");
        }

        iniciarIntentoWiFi();

        // Fuerza el primer heartbeat apenas el WiFi quede conectado.
        ultimoHeartbeat_ = millis() - INTERVALO_HEARTBEAT;
    }

    // Debe llamarse en cada vuelta del loop. No usa delay().
    void actualizar()
    {
        mantenerWiFi();
        mantenerHeartbeat();
    }

    // Genera automaticamente un ID de evento y conserva el mismo ID durante
    // los tres intentos. Devuelve true solo si la ESP32 confirma con HTTP 2xx.
    bool enviarHallazgo(uint8_t objetoId, uint8_t confianza)
    {
        uint32_t evento = generarEventoId();
        return enviarHallazgo(objetoId, confianza, evento);
    }

    // Variante para conservar un ID propio. Util si otra parte del programa ya
    // genera un identificador unico. Si evento es 0 se crea uno automaticamente.
    bool enviarHallazgo(
        uint8_t objetoId,
        uint8_t confianza,
        uint32_t evento
    )
    {
        if (objetoId < 1 || objetoId > 6 || confianza > 100)
        {
            ultimoCodigoHTTP_ = -20;

            if (diagnosticoSerial_)
            {
                Serial.println("[ATOM.IA] Hallazgo rechazado: datos invalidos");
            }

            return false;
        }

        if (evento == 0)
        {
            evento = generarEventoId();
        }

        if (evento > ultimoEventoGenerado_)
        {
            ultimoEventoGenerado_ = evento;
        }

        ultimoEventoEnviado_ = evento;

        if (diagnosticoSerial_)
        {
            Serial.println();
            Serial.println("[ATOM.IA] Enviando hallazgo");
            Serial.print("[ATOM.IA] Objeto ID: ");
            Serial.println(objetoId);
            Serial.print("[ATOM.IA] Confianza: ");
            Serial.print(confianza);
            Serial.println("%");
            Serial.print("[ATOM.IA] Evento: ");
            Serial.println(evento);
        }

        for (uint8_t intento = 1; intento <= MAX_INTENTOS_EVENTO; intento++)
        {
            if (diagnosticoSerial_)
            {
                Serial.print("[ATOM.IA] Intento ");
                Serial.print(intento);
                Serial.print("/");
                Serial.println(MAX_INTENTOS_EVENTO);
            }

            ultimoCodigoHTTP_ = enviarEventoUnaVez(
                objetoId,
                confianza,
                evento
            );

            if (ultimoCodigoHTTP_ >= 200 && ultimoCodigoHTTP_ < 300)
            {
                bool cambio = !servidorOnline_;
                servidorOnline_ = true;
                fallosHeartbeat_ = 0;
                ultimoHeartbeatCorrecto_ = millis();
                cambioEstado_ = cambioEstado_ || cambio;

                if (diagnosticoSerial_)
                {
                    Serial.println("[ATOM.IA] Evento confirmado por la ESP32");
                }

                return true;
            }

            if (diagnosticoSerial_)
            {
                Serial.print("[ATOM.IA] HTTP/error: ");
                Serial.println(ultimoCodigoHTTP_);
            }

            if (intento < MAX_INTENTOS_EVENTO)
            {
                delay(140);
            }
        }

        if (diagnosticoSerial_)
        {
            Serial.println("[ATOM.IA] Envio no confirmado");
        }

        return false;
    }

    bool wifiConectado() const
    {
        return WiFi.status() == WL_CONNECTED;
    }

    bool online() const
    {
        return wifiConectado() && servidorOnline_;
    }

    Estado estado() const
    {
        if (!wifiConectado())
        {
            return SIN_WIFI;
        }

        return servidorOnline_ ? ATOM_IA_ONLINE : ESPERANDO_ATOM_IA;
    }

    const char* estadoTexto() const
    {
        switch (estado())
        {
            case ATOM_IA_ONLINE:
                return "ONLINE";

            case ESPERANDO_ATOM_IA:
                return "ESPERANDO";

            default:
                return "SIN WIFI";
        }
    }

    // Devuelve true una vez cuando cambia WiFi/ATOM.ia. Sirve para actualizar
    // una pantalla sin redibujarla en cada vuelta del loop.
    bool consumirCambioEstado()
    {
        bool cambio = cambioEstado_;
        cambioEstado_ = false;
        return cambio;
    }

    uint8_t fallosHeartbeat() const
    {
        return fallosHeartbeat_;
    }

    uint32_t ultimoHeartbeatCorrecto() const
    {
        return ultimoHeartbeatCorrecto_;
    }

    uint32_t ultimoEvento() const
    {
        return ultimoEventoEnviado_;
    }

    int ultimoCodigoHTTP() const
    {
        return ultimoCodigoHTTP_;
    }

    IPAddress ipLocal() const
    {
        return WiFi.localIP();
    }

private:
    // Parametros que ya fueron probados con ATOM.IA DIRECTO V3.
    enum : uint32_t
    {
        ESP32_PORT = 80,
        INTERVALO_RECONEXION_WIFI = 6000,
        INTERVALO_HEARTBEAT = 1800,
        TIMEOUT_HTTP_HEARTBEAT = 900,
        TIMEOUT_HTTP_EVENTO = 1600,
        MAX_FALLOS_HEARTBEAT = 3,
        MAX_INTENTOS_EVENTO = 3
    };

    IPAddress esp32IP_;

    bool diagnosticoSerial_ = true;
    bool wifiConectadoAnterior_ = false;
    bool servidorOnline_ = false;
    bool cambioEstado_ = false;

    uint8_t fallosHeartbeat_ = 0;
    uint32_t ultimoIntentoWiFi_ = 0;
    uint32_t ultimoHeartbeat_ = 0;
    uint32_t ultimoHeartbeatCorrecto_ = 0;
    uint32_t ultimoEventoGenerado_ = 0;
    uint32_t ultimoEventoEnviado_ = 0;
    int ultimoCodigoHTTP_ = 0;

    void iniciarIntentoWiFi()
    {
        ultimoIntentoWiFi_ = millis();

        if (diagnosticoSerial_)
        {
            Serial.println("[ATOM.IA] Conectando al WiFi...");
        }

        WiFi.begin("ATOM-ARCHBOT", "ATOM2026-Harnero!");
    }

    void mantenerWiFi()
    {
        bool conectado = wifiConectado();

        if (conectado && !wifiConectadoAnterior_)
        {
            wifiConectadoAnterior_ = true;
            servidorOnline_ = false;
            fallosHeartbeat_ = 0;
            cambioEstado_ = true;
            ultimoHeartbeat_ = millis() - INTERVALO_HEARTBEAT;

            if (diagnosticoSerial_)
            {
                Serial.print("[ATOM.IA] WiFi conectado. IP MATRIX: ");
                Serial.println(WiFi.localIP());
            }
        }

        if (!conectado && wifiConectadoAnterior_)
        {
            wifiConectadoAnterior_ = false;
            servidorOnline_ = false;
            fallosHeartbeat_ = 0;
            cambioEstado_ = true;

            if (diagnosticoSerial_)
            {
                Serial.println("[ATOM.IA] WiFi desconectado");
            }
        }

        if (
            !conectado &&
            millis() - ultimoIntentoWiFi_ >= INTERVALO_RECONEXION_WIFI
        )
        {
            iniciarIntentoWiFi();
        }
    }

    void mantenerHeartbeat()
    {
        if (millis() - ultimoHeartbeat_ < INTERVALO_HEARTBEAT)
        {
            return;
        }

        ultimoHeartbeat_ = millis();

        if (!wifiConectado())
        {
            return;
        }

        bool ok = enviarHeartbeat();

        if (ok)
        {
            bool cambio = !servidorOnline_;
            servidorOnline_ = true;
            fallosHeartbeat_ = 0;
            ultimoHeartbeatCorrecto_ = millis();
            cambioEstado_ = cambioEstado_ || cambio;

            if (cambio && diagnosticoSerial_)
            {
                Serial.println("[ATOM.IA] Heartbeat OK. Servidor online");
            }

            return;
        }

        if (fallosHeartbeat_ < 255)
        {
            fallosHeartbeat_++;
        }

        if (diagnosticoSerial_)
        {
            Serial.print("[ATOM.IA] Heartbeat sin respuesta ");
            Serial.print(fallosHeartbeat_);
            Serial.print("/");
            Serial.println(MAX_FALLOS_HEARTBEAT);
        }

        if (fallosHeartbeat_ >= MAX_FALLOS_HEARTBEAT)
        {
            bool cambio = servidorOnline_;
            servidorOnline_ = false;
            cambioEstado_ = cambioEstado_ || cambio;
        }
    }

    bool enviarHeartbeat()
    {
        if (!wifiConectado())
        {
            return false;
        }

        WiFiClient cliente;

        if (!cliente.connect(esp32IP_, ESP32_PORT))
        {
            ultimoCodigoHTTP_ = -11;
            return false;
        }

        const char body[] = "{}";

        cliente.println("POST /api/archbot/heartbeat HTTP/1.1");
        cliente.print("Host: ");
        cliente.println(esp32IP_);
        cliente.println("X-ATOM-Key: A7mQ2xP9kL4vR8sT6cN3");
        cliente.println("Content-Type: application/json");
        cliente.print("Content-Length: ");
        cliente.println(strlen(body));
        cliente.println("Connection: close");
        cliente.println();
        cliente.print(body);

        ultimoCodigoHTTP_ = leerCodigoHTTP(
            cliente,
            TIMEOUT_HTTP_HEARTBEAT
        );

        cerrarCliente(cliente);

        return ultimoCodigoHTTP_ >= 200 && ultimoCodigoHTTP_ < 300;
    }

    int enviarEventoUnaVez(
        uint8_t objetoId,
        uint8_t confianza,
        uint32_t evento
    )
    {
        if (!wifiConectado())
        {
            return -10;
        }

        WiFiClient cliente;

        if (!cliente.connect(esp32IP_, ESP32_PORT))
        {
            return -11;
        }

        String body;
        body.reserve(100);
        body = "{\"estado\":1,\"objeto_id\":";
        body += String(objetoId);
        body += ",\"confianza\":";
        body += String(confianza);
        body += ",\"evento\":";
        body += String(evento);
        body += "}";

        cliente.println("POST /api/archbot/event HTTP/1.1");
        cliente.print("Host: ");
        cliente.println(esp32IP_);
        cliente.println("X-ATOM-Key: A7mQ2xP9kL4vR8sT6cN3");
        cliente.println("Content-Type: application/json");
        cliente.print("Content-Length: ");
        cliente.println(body.length());
        cliente.println("Connection: close");
        cliente.println();
        cliente.print(body);

        int codigo = leerCodigoHTTP(cliente, TIMEOUT_HTTP_EVENTO);
        cerrarCliente(cliente);
        return codigo;
    }

    int leerCodigoHTTP(WiFiClient &cliente, uint32_t timeoutMs)
    {
        uint32_t inicio = millis();

        while (
            !cliente.available() &&
            cliente.connected() &&
            millis() - inicio < timeoutMs
        )
        {
            delay(1);
        }

        // El servidor puede cerrar justo despues de dejar la respuesta.
        while (
            !cliente.available() &&
            millis() - inicio < timeoutMs
        )
        {
            delay(1);
        }

        if (!cliente.available())
        {
            return -1;
        }

        String linea = cliente.readStringUntil('\n');
        linea.trim();

        if (!linea.startsWith("HTTP/"))
        {
            return -1;
        }

        int espacio1 = linea.indexOf(' ');

        if (espacio1 < 0)
        {
            return -1;
        }

        int espacio2 = linea.indexOf(' ', espacio1 + 1);
        String codigoTexto = espacio2 > 0
            ? linea.substring(espacio1 + 1, espacio2)
            : linea.substring(espacio1 + 1);

        return codigoTexto.toInt();
    }

    void cerrarCliente(WiFiClient &cliente)
    {
        uint32_t inicio = millis();

        while (millis() - inicio < 40)
        {
            while (cliente.available())
            {
                cliente.read();
            }

            if (!cliente.connected())
            {
                break;
            }

            delay(1);
        }

        cliente.stop();
    }

    uint32_t generarEventoId()
    {
        uint32_t nuevo = millis();

        if (nuevo == 0)
        {
            nuevo = 1;
        }

        if (nuevo <= ultimoEventoGenerado_)
        {
            nuevo = ultimoEventoGenerado_ + 1;
        }

        ultimoEventoGenerado_ = nuevo;
        return nuevo;
    }
};
