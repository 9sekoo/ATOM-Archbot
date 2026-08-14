# ATOM ArchBot

ATOM ArchBot es un proyecto de robótica orientado al apoyo de procesos de harneado arqueológico mediante visión artificial, sensores, control automático de movimiento y sistemas de aviso.

El objetivo del proyecto es desarrollar un sistema capaz de realizar un proceso de harneado controlado, adaptar automáticamente la velocidad de movimiento según la cantidad de tierra presente y detener el proceso cuando se detecte un posible objeto arqueológico.

El sistema utiliza un MATRIX Mini R4 como controlador principal y una cámara M-Vision como sistema de visión.

---

# Objetivo general

ATOM busca automatizar parte del proceso de harneado de tierra de manera controlada y segura.

El sistema debe ser capaz de:

- Iniciar el proceso solamente cuando el usuario lo indique.
- Detectar la cantidad aproximada de tierra presente.
- Regular la velocidad de oscilación del sistema de harneado.
- Reducir la velocidad cuando exista menos material.
- Evitar movimientos bruscos que puedan dañar posibles restos arqueológicos.
- Detectar posibles objetos mediante visión artificial.
- Detener automáticamente el harneado ante un hallazgo.
- Alertar al usuario mediante luces, sonido y pantallas.
- Permitir pausar o reanudar el proceso.
- Controlar automáticamente el movimiento de una bandeja de tierra.
- Detectar errores de inicialización antes de comenzar.

---

# Control principal

El controlador principal de ATOM es:

- MATRIX Mini R4

El MATRIX se encarga de coordinar:

- Cámara M-Vision
- Sensores
- Servomotores
- Motores DC
- LEDs RGB
- Buzzer
- Pantallas
- Pulsadores
- Final de carrera
- Estados generales del sistema

---

# Componentes principales

## Control

- 1 MATRIX Mini R4

## Visión

- 1 M-Vision Cam

## Movimiento

- 2 servomotores MATRIX con engranajes metálicos
- 2 motores DC MATRIX

## Sensores

- 2 sensores láser MATRIX
- 1 final de carrera MATRIX

## Controles físicos

- 4 pulsadores
- 1 pulsador con bloqueo para encendido y apagado

## Indicadores

- 4 tiras LED RGB
- Buzzer integrado del MATRIX
- Botones integrados del MATRIX

## Pantallas

- 1 pantalla LCD I2C 16x2
- 1 pantalla LCD I2C 16x4
- Pantalla OLED integrada del MATRIX Mini R4

## Alimentación

- 6 baterías 18650
- Portabaterías
- Módulo de carga 2S
- BMS 2S

---

# Arquitectura general

El sistema funciona con una arquitectura centralizada en el MATRIX Mini R4.

```text
                 ┌───────────────────┐
                 │   M-VISION CAM    │
                 │ Visión artificial │
                 └─────────┬─────────┘
                           │
                          UART
                           │
                           ▼
                 ┌───────────────────┐
                 │ MATRIX MINI R4    │
                 │ Control principal │
                 └─────────┬─────────┘
                           │
         ┌─────────────────┼─────────────────┐
         │                 │                 │
         ▼                 ▼                 ▼
   Sensores láser      Servomotores       Pantallas
         │                 │                 │
         ├───────────── Motores DC           │
         │                                   │
         ├───────────── LEDs RGB              │
         │                                   │
         └───────────── Buzzer                │
