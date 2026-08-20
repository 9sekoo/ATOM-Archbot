#include <MatrixMiniR4.h>

#include "CAMARA_MVISION.h"
#include "COMUNICACION_ATOM_IA.h"
#include "PANTALLA_ESTADOS.h"

/*
  ============================================================
  ATOM-ARCHBOT - PROGRAMA PRINCIPAL DEL ROBOT
  ============================================================

  Este es el coordinador general. Las otras tres pestanas se ocupan de:

    CAMARA_MVISION.h       -> UART, datos y eventos de M-Vision.
    COMUNICACION_ATOM_IA.h -> WiFi, heartbeat y eventos para ATOM.ia.
    PANTALLA_ESTADOS.h     -> OLED, RGB, buzzer y animaciones.

  Aqui agregaremos gradualmente motores, servos, sensores laser, final de
  carrera y la maquina de estados definitiva del harneado.
*/

CamaraMVision Camara;
AtomIAComunicacion AtomIA;
PantallaEstados Pantalla;

EstadoSistema estadoRobot = INICIALIZANDO;


// ============================================================
// CONFIGURACION TEMPORAL DE PRUEBA
// ============================================================

// Permite probar el flujo completo aunque la M-Vision todavia no tenga su
// nuevo programa. En estado LISTO, el boton derecho abajo simula una PUNTA 91%.
// Cuando la camara quede terminada cambiaremos este valor a false.
const bool PERMITIR_HALLAZGO_PRUEBA_BOTON = true;

const uint32_t TIEMPO_INICIALIZACION_MS = 1500;
const uint32_t DEBOUNCE_BOTONES_MS = 180;

uint32_t inicioSistemaMs = 0;
uint32_t ultimoBotonArribaMs = 0;
uint32_t ultimoBotonAbajoMs = 0;
bool botonArribaAnterior = false;
bool botonAbajoAnterior = false;


// ============================================================
// CONTROL SEGURO DE ACTUADORES
// ============================================================

void detenerActuadoresSeguro()
{
    /*
      En la siguiente etapa agregaremos aqui:

        - motores de bandeja en velocidad 0;
        - servos del harnero a posicion segura/0 grados;
        - cancelacion de movimientos pendientes;

      Esta funcion se llama antes de anunciar un hallazgo, una pausa o error.
      Por ahora no mueve ningun puerto para evitar activar hardware sin haber
      comprobado primero las conexiones y la alimentacion.
    */
}


void iniciarActuadoresHarneado()
{
    /*
      Aqui se agregara el movimiento real de los dos servos del harnero y su
      velocidad segun los sensores laser. Por ahora solo cambia el estado.
    */
}


// ============================================================
// MAQUINA DE ESTADOS DEL ROBOT
// ============================================================

void cambiarEstadoRobot(EstadoSistema nuevoEstado)
{
    if (estadoRobot == nuevoEstado)
    {
        return;
    }

    estadoRobot = nuevoEstado;

    if (
        nuevoEstado == PAUSADO ||
        nuevoEstado == HALLAZGO ||
        nuevoEstado == FINALIZADO ||
        nuevoEstado == ERROR_SISTEMA
    )
    {
        detenerActuadoresSeguro();
    }

    if (nuevoEstado == HARNIENDO)
    {
        iniciarActuadoresHarneado();
    }

    Pantalla.cambiarEstado(nuevoEstado);

    Serial.print("[ROBOT] Estado: ");
    Serial.println(PantallaEstados::nombreEstado(nuevoEstado));
}


void actualizarInicializacion()
{
    if (
        estadoRobot == INICIALIZANDO &&
        millis() - inicioSistemaMs >= TIEMPO_INICIALIZACION_MS
    )
    {
        // En la version definitiva este cambio ocurrira solo cuando terminen
        // las pruebas de camara, sensores, servos, pantallas y final de carrera.
        cambiarEstadoRobot(LISTO);
    }
}


// ============================================================
// FLUJO DE HALLAZGO: CAMARA -> MATRIX -> ATOM.IA
// ============================================================

void procesarHallazgo(const DatosMVision &hallazgo)
{
    if (!hallazgo.valido || !hallazgo.detectado)
    {
        return;
    }

    // Primero se detiene el movimiento; despues se informa y se comunica.
    detenerActuadoresSeguro();

    Pantalla.actualizarDatosCamara(
        hallazgo.objetoId,
        hallazgo.confianza,
        hallazgo.eventoCamara
    );

    cambiarEstadoRobot(HALLAZGO);
    Pantalla.mostrarEnvioATOMIA();
    Pantalla.actualizar();

    // ATOM.ia genera su propio ID grande y lo conserva en los tres reintentos.
    // El evento de la camara solo sirve para no procesar dos veces la misma
    // deteccion dentro del MATRIX.
    bool recibido = AtomIA.enviarHallazgo(
        hallazgo.objetoId,
        hallazgo.confianza
    );

    Pantalla.mostrarResultadoEnvio(
        recibido,
        AtomIA.ultimoEvento(),
        AtomIA.ultimoCodigoHTTP()
    );

    Serial.print("[ROBOT] Hallazgo ");
    Serial.print(CamaraMVision::nombreObjeto(hallazgo.objetoId));
    Serial.print(" · ");
    Serial.print(hallazgo.confianza);
    Serial.print("% · ATOM.ia ");
    Serial.println(recibido ? "CONFIRMADO" : "NO CONFIRMADO");
}


void revisarHallazgosCamara()
{
    if (!Camara.hayNuevoHallazgo())
    {
        return;
    }

    DatosMVision hallazgo = Camara.consumirHallazgo();

    // La camara solo controla el robot durante harneado. Si llega un paquete
    // mientras estamos en otro estado, se registra pero no inicia otro flujo.
    if (estadoRobot == HARNIENDO)
    {
        procesarHallazgo(hallazgo);
    }
    else
    {
        Serial.println("[ROBOT] Hallazgo de camara ignorado: robot no harneando");
    }
}


void simularHallazgoPrueba()
{
    DatosMVision prueba;
    prueba.valido = true;
    prueba.detectado = true;
    prueba.objetoId = 2;
    prueba.confianza = 91;
    prueba.eventoCamara = millis();
    prueba.recibidoMs = millis();

    procesarHallazgo(prueba);
}


// ============================================================
// BOTONES INTEGRADOS - CONTROL TEMPORAL
// ============================================================

void accionBotonArriba()
{
    if (estadoRobot == LISTO)
    {
        cambiarEstadoRobot(HARNIENDO);
    }
    else if (estadoRobot == HARNIENDO)
    {
        cambiarEstadoRobot(PAUSADO);
    }
    else if (estadoRobot == PAUSADO)
    {
        cambiarEstadoRobot(HARNIENDO);
    }
}


void accionBotonAbajo()
{
    if (estadoRobot == HALLAZGO)
    {
        // En la version final este boton confirmara que una persona ya reviso
        // el posible hallazgo antes de permitir continuar.
        cambiarEstadoRobot(LISTO);
    }
    else if (
        estadoRobot == LISTO &&
        PERMITIR_HALLAZGO_PRUEBA_BOTON
    )
    {
        simularHallazgoPrueba();
    }
}


void actualizarBotones()
{
    bool arriba = MiniR4.BTN_UP.getState();

    if (
        arriba &&
        !botonArribaAnterior &&
        millis() - ultimoBotonArribaMs >= DEBOUNCE_BOTONES_MS
    )
    {
        ultimoBotonArribaMs = millis();
        accionBotonArriba();
    }

    botonArribaAnterior = arriba;

    bool abajo = MiniR4.BTN_DOWN.getState();

    if (
        abajo &&
        !botonAbajoAnterior &&
        millis() - ultimoBotonAbajoMs >= DEBOUNCE_BOTONES_MS
    )
    {
        ultimoBotonAbajoMs = millis();
        accionBotonAbajo();
    }

    botonAbajoAnterior = abajo;
}


// ============================================================
// SETUP Y LOOP PRINCIPALES
// ============================================================

void setup()
{
    MiniR4.begin();
    Serial.begin(115200);

    Pantalla.comenzar();
    AtomIA.comenzar();
    Camara.comenzar();

    inicioSistemaMs = millis();
    estadoRobot = INICIALIZANDO;

    Serial.println();
    Serial.println("================================");
    Serial.println("ATOM-ARCHBOT · 4 PESTANAS");
    Serial.println("Robot + Camara + ATOM.ia + UI");
    Serial.println("================================");
}


void loop()
{
    // Los dos enlaces se mantienen independientemente del estado del robot.
    AtomIA.actualizar();
    Camara.actualizar();

    // La pantalla solo recibe estados y datos; no toma decisiones del robot.
    Pantalla.actualizarConexiones(
        AtomIA.wifiConectado(),
        AtomIA.online(),
        Camara.conectada()
    );

    if (Camara.ultimoDato().valido)
    {
        const DatosMVision &dato = Camara.ultimoDato();
        Pantalla.actualizarDatosCamara(
            dato.objetoId,
            dato.confianza,
            dato.eventoCamara
        );
    }

    actualizarInicializacion();
    revisarHallazgosCamara();
    actualizarBotones();
    Pantalla.actualizar();

    delay(2);
}

