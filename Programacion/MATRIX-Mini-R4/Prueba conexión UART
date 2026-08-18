#include <MatrixMiniR4.h>


// ─────────────────────────────────────────────
// ATOM ARCHBOT - M-VISION + MATRIX
// Vision + OLED + RGB + Sonido + Inicio/Pausa
// ─────────────────────────────────────────────

unsigned int datosCamara[20];


// ─────────────────────────────────────────────
// Estado general
// ─────────────────────────────────────────────

bool sistemaIniciado = false;
bool sistemaPausado = false;

bool objetoDetectado = false;
bool estadoAnterior = false;

unsigned int confianzaActual = 0;


// ─────────────────────────────────────────────
// Botón superior derecho
// ─────────────────────────────────────────────

bool botonAnterior = false;

unsigned long ultimoCambioBoton = 0;

const unsigned long DEBOUNCE_BOTON = 180;


// ─────────────────────────────────────────────
// RGB
// ─────────────────────────────────────────────

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


void rgbPausa()
{
    MiniR4.LED.setColor(1, 255, 80, 0);
    MiniR4.LED.setColor(2, 255, 80, 0);
}


void rgbApagado()
{
    MiniR4.LED.setColor(1, 0, 0, 0);
    MiniR4.LED.setColor(2, 0, 0, 0);
}


// ─────────────────────────────────────────────
// Pantalla - Esperando inicio
// ─────────────────────────────────────────────

void pantallaInicio()
{
    MiniR4.OLED.clearDisplay();

    MiniR4.OLED.setTextSize(1);

    MiniR4.OLED.setCursor(15, 2);
    MiniR4.OLED.print("ATOM ARCHBOT");

    MiniR4.OLED.setCursor(7, 17);
    MiniR4.OLED.print("PRESIONA BOTON");

    MiniR4.OLED.setCursor(13, 27);
    MiniR4.OLED.print("PARA COMENZAR");

    MiniR4.OLED.display();
}


// ─────────────────────────────────────────────
// Pantalla - Buscando
// ─────────────────────────────────────────────

void pantallaBuscando()
{
    MiniR4.OLED.clearDisplay();

    MiniR4.OLED.setTextSize(1);

    MiniR4.OLED.setCursor(15, 3);
    MiniR4.OLED.print("ATOM ARCHBOT");

    MiniR4.OLED.setCursor(8, 21);
    MiniR4.OLED.print("Buscando objeto...");

    MiniR4.OLED.display();
}


// ─────────────────────────────────────────────
// Pantalla - Detectado
// ─────────────────────────────────────────────

void pantallaDetectado(unsigned int confianza)
{
    MiniR4.OLED.clearDisplay();

    MiniR4.OLED.setTextSize(1);

    MiniR4.OLED.setCursor(10, 2);
    MiniR4.OLED.print("OBJETO DETECTADO");

    MiniR4.OLED.setCursor(18, 20);
    MiniR4.OLED.print("Confianza: ");

    MiniR4.OLED.print(confianza);
    MiniR4.OLED.print("%");

    MiniR4.OLED.display();
}


// ─────────────────────────────────────────────
// Pantalla - Pausa
// ─────────────────────────────────────────────

void pantallaPausa()
{
    MiniR4.OLED.clearDisplay();

    MiniR4.OLED.setTextSize(2);

    MiniR4.OLED.setCursor(18, 2);
    MiniR4.OLED.print("PAUSADO");

    MiniR4.OLED.setTextSize(1);

    MiniR4.OLED.setCursor(8, 24);
    MiniR4.OLED.print("Pulsa para seguir");

    MiniR4.OLED.display();
}


// ─────────────────────────────────────────────
// Sonido mientras busca
// ─────────────────────────────────────────────
//
// pi ... pi ... pi ...
// ─────────────────────────────────────────────

unsigned long ultimoPi = 0;

const unsigned long INTERVALO_PI = 750;
const unsigned int DURACION_PI = 55;


void sonidoBuscando()
{
    unsigned long ahora = millis();

    if (ahora - ultimoPi >= INTERVALO_PI)
    {
        ultimoPi = ahora;

        MiniR4.Buzzer.Tone(
            NOTE_C5,
            DURACION_PI
        );
    }
}


// ─────────────────────────────────────────────
// Melodía divertida de detección
// ─────────────────────────────────────────────
//
// Suena como:
//
// ta ta ta TAAA
// ti-ri-ri
// TIN TIN TAAA
//
// y se repite mientras la pieza siga detectada.
// ─────────────────────────────────────────────

const int melodia[] =
{
    NOTE_C5,
    NOTE_E5,
    NOTE_G5,
    NOTE_C6,

    NOTE_G5,
    NOTE_E5,
    NOTE_G5,

    NOTE_A5,
    NOTE_G5,
    NOTE_E5,

    NOTE_C6,
    NOTE_E6,
    NOTE_G6,

    NOTE_E6,
    NOTE_C6
};


const int duraciones[] =
{
    90,
    90,
    90,
    220,

    80,
    80,
    120,

    90,
    90,
    140,

    100,
    100,
    260,

    100,
    320
};


const int TOTAL_NOTAS =
    sizeof(melodia) / sizeof(melodia[0]);


int notaActual = 0;

unsigned long inicioNota = 0;

bool notaSonando = false;


// ─────────────────────────────────────────────
// Reiniciar melodía
// ─────────────────────────────────────────────

void reiniciarMelodia()
{
    MiniR4.Buzzer.NoTone();

    notaActual = 0;

    inicioNota = millis();

    notaSonando = false;
}


// ─────────────────────────────────────────────
// Reproducir melodía sin bloquear
// ─────────────────────────────────────────────

void sonidoDetectado()
{
    unsigned long ahora = millis();


    if (!notaSonando)
    {
        MiniR4.Buzzer.Tone(
            melodia[notaActual],
            duraciones[notaActual]
        );

        inicioNota = ahora;

        notaSonando = true;
    }


    if (
        notaSonando &&
        ahora - inicioNota >=
        (unsigned long)(duraciones[notaActual] + 25)
    )
    {
        MiniR4.Buzzer.NoTone();

        notaSonando = false;

        notaActual++;


        if (notaActual >= TOTAL_NOTAS)
        {
            notaActual = 0;
        }
    }
}


// ─────────────────────────────────────────────
// Actualizar botón
// ─────────────────────────────────────────────

void actualizarBoton()
{
    bool botonActual =
        MiniR4.BTN_UP.getState();


    if (
        botonActual &&
        !botonAnterior &&
        millis() - ultimoCambioBoton >= DEBOUNCE_BOTON
    )
    {
        ultimoCambioBoton = millis();


        // ─────────────────────────────────────
        // Primera pulsación: INICIAR
        // ─────────────────────────────────────

        if (!sistemaIniciado)
        {
            sistemaIniciado = true;
            sistemaPausado = false;

            objetoDetectado = false;
            estadoAnterior = false;

            reiniciarMelodia();

            rgbRojo();

            pantallaBuscando();

            ultimoPi = millis();

            Serial.println(
                "Sistema iniciado"
            );
        }


        // ─────────────────────────────────────
        // Si ya comenzó: PAUSA / CONTINUAR
        // ─────────────────────────────────────

        else
        {
            sistemaPausado =
                !sistemaPausado;


            MiniR4.Buzzer.NoTone();

            reiniciarMelodia();


            if (sistemaPausado)
            {
                rgbPausa();

                pantallaPausa();

                Serial.println(
                    "Sistema pausado"
                );
            }

            else
            {
                objetoDetectado = false;
                estadoAnterior = false;

                rgbRojo();

                pantallaBuscando();

                ultimoPi = millis();

                Serial.println(
                    "Sistema reanudado"
                );
            }
        }
    }


    botonAnterior = botonActual;
}


// ─────────────────────────────────────────────
// Actualizar visuales
// ─────────────────────────────────────────────

void actualizarEstadoVisual()
{
    if (objetoDetectado != estadoAnterior)
    {
        if (objetoDetectado)
        {
            rgbVerde();

            pantallaDetectado(
                confianzaActual
            );

            reiniciarMelodia();
        }

        else
        {
            rgbRojo();

            pantallaBuscando();

            reiniciarMelodia();

            ultimoPi = millis();
        }


        estadoAnterior =
            objetoDetectado;
    }
}


// ─────────────────────────────────────────────
// Inicio
// ─────────────────────────────────────────────

void setup()
{
    MiniR4.begin();

    Serial.begin(115200);


    // Comunicación M-Vision

    MiniR4.Vision.Begin();


    // Brillo RGB

    MiniR4.LED.setBrightness(1, 80);
    MiniR4.LED.setBrightness(2, 80);


    // ─────────────────────────────────────────
    // Al encender NO comienza a detectar
    // ─────────────────────────────────────────

    sistemaIniciado = false;
    sistemaPausado = false;

    objetoDetectado = false;
    estadoAnterior = false;


    rgbApagado();

    pantallaInicio();


    Serial.println();
    Serial.println("ATOM ARCHBOT");

    Serial.println(
        "Esperando inicio..."
    );
}


// ─────────────────────────────────────────────
// Programa principal
// ─────────────────────────────────────────────

void loop()
{
    // El botón funciona siempre

    actualizarBoton();


    // ─────────────────────────────────────────
    // Todavía no ha comenzado
    // ─────────────────────────────────────────

    if (!sistemaIniciado)
    {
        MiniR4.Buzzer.NoTone();

        delay(5);

        return;
    }


    // ─────────────────────────────────────────
    // Sistema pausado
    // ─────────────────────────────────────────

    if (sistemaPausado)
    {
        MiniR4.Buzzer.NoTone();

        delay(5);

        return;
    }


    // ─────────────────────────────────────────
    // Leer M-Vision
    // ─────────────────────────────────────────

    int cantidad =
        MiniR4.Vision.SmartCamReader(
            datosCamara,
            10
        );


    if (cantidad >= 6)
    {
        unsigned int estado =
            datosCamara[0];

        unsigned int objeto =
            datosCamara[1];

        unsigned int confianza =
            datosCamara[2];

        unsigned int posicionX =
            datosCamara[3];

        unsigned int posicionY =
            datosCamara[4];

        unsigned int evento =
            datosCamara[5];


        confianzaActual =
            confianza;


        objetoDetectado =
            (estado == 1);


        // ─────────────────────────────────────
        // Monitor serial
        // ─────────────────────────────────────

        Serial.print("Estado: ");
        Serial.print(estado);

        Serial.print(" | ID: ");
        Serial.print(objeto);

        Serial.print(" | Confianza: ");
        Serial.print(confianza);
        Serial.print("%");

        Serial.print(" | X: ");
        Serial.print(posicionX);

        Serial.print(" | Y: ");
        Serial.print(posicionY);

        Serial.print(" | Evento: ");
        Serial.println(evento);
    }


    // ─────────────────────────────────────────
    // Pantalla + RGB
    // ─────────────────────────────────────────

    actualizarEstadoVisual();


    // ─────────────────────────────────────────
    // Sonido
    // ─────────────────────────────────────────

    if (objetoDetectado)
    {
        sonidoDetectado();
    }

    else
    {
        sonidoBuscando();
    }


    delay(2);
}
