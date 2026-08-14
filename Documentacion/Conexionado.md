# Conexiones de ATOM ArchBot

Este documento describe la distribución de conexiones entre el MATRIX Mini R4 y los distintos componentes de ATOM ArchBot.

La distribución mostrada corresponde al esquema actual del prototipo y puede cambiar durante las pruebas finales.

---

# Controlador principal

El controlador central del sistema es:

**MATRIX Mini R4**

Desde este controlador se conectan:

- Cámara M-Vision
- Sensores láser
- Servomotores
- Motores DC
- Pulsadores
- Final de carrera
- Pantallas LCD
- Sistema RGB
- Buzzer y botones integrados

---

# Resumen general de conexiones

| Componente | Puerto / conexión | Tipo | Función |
|---|---|---|---|
| M-Vision Cam | UART | Comunicación | Visión artificial |
| Sensor láser izquierdo | I2C 1 | Entrada / sensor | Medición de tierra |
| Sensor láser derecho | I2C 2 | Entrada / sensor | Medición de tierra |
| LCD 16x2 | I2C 3 | Salida / pantalla | Interfaz principal |
| LCD 16x4 | I2C 4 | Salida / pantalla | Información detallada |
| Servo izquierdo | SERVO 1 | Salida | Movimiento del harnero |
| Servo derecho | SERVO 2 | Salida | Movimiento del harnero |
| Motor DC izquierdo | MOTOR 1 | Salida | Movimiento de bandeja |
| Motor DC derecho | MOTOR 2 | Salida | Movimiento de bandeja |
| Pulsador 1 | Entrada digital | Input | Comenzar harneado |
| Pulsador 2 | Entrada digital | Input | Pausa / continuar |
| Pulsador 3 | Entrada digital | Input | Sacar bandeja |
| Pulsador 4 | Entrada digital | Input | Insertar bandeja |
| Final de carrera | Entrada digital | Input | Detener inserción |
| RGB externo | Puerto derecho 1 | Salida | Indicador visual |

---

# Cámara M-Vision

## Conexión

La cámara M-Vision se conecta al puerto:

```text
UART
