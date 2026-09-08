# 🔌 Conexionado real de ATOM ArchBot V3

Este documento describe el **conexionado físico real utilizado en ATOM ArchBot V3** con el **MATRIX Mini R4**.

> **Importante:** Los nombres `-L` y `-R` indican los conectores ubicados en el lado izquierdo y derecho del MATRIX Mini R4, respectivamente.

---

## 🧠 Controlador principal

El controlador central del sistema es el:

### **MATRIX Mini R4**

El MATRIX Mini R4 recibe las señales de los sensores y pulsadores, controla los actuadores y administra las interfaces visuales y sonoras del robot.

Componentes conectados:

- Cámara **M-Vision**
- 2 sensores láser ToF
- 2 servomotores del harnero
- 2 motores DC de la bandeja
- 4 pulsadores externos
- 1 final de carrera
- LCD 16×2
- LCD 16×4
- 2 barras RGB WS2812B de 8 LEDs
- OLED integrada
- Buzzer integrado
- Botones integrados del MATRIX

---

# 📋 Mapa general de conexiones

| Componente | Puerto real | Tipo | Función |
|---|---|---|---|
| M-Vision Cam | UART / puerto M-Vision | Comunicación | Visión artificial |
| Sensor ToF izquierdo | **I2C1** | Sensor | Medición de distancia / cantidad de tierra |
| LCD 16×2 | **I2C2** | Pantalla | Mensajes principales al usuario |
| LCD 16×4 | **I2C3** | Pantalla | Datos detallados del proceso |
| Sensor ToF derecho | **I2C4** | Sensor | Medición de distancia / cantidad de tierra |
| Servo izquierdo | **SERVO 1** | Actuador | Movimiento del harnero |
| Servo derecho | **SERVO 2** | Actuador | Movimiento del harnero |
| Motor DC izquierdo | **M1 / MOTOR 1** | Actuador | Movimiento de bandeja |
| Motor DC derecho | **M2 / MOTOR 2** | Actuador | Movimiento de bandeja |
| Pulsador 1 | **D1-L** | Entrada digital | Comenzar harneado |
| Pulsador 2 | **D2-L** | Entrada digital | Pausar / continuar |
| Pulsador 3 | **D3-L** | Entrada digital | Sacar bandeja |
| Pulsador 4 | **D4-L** | Entrada digital | Insertar bandeja |
| Final de carrera | **A1-L** | Entrada digital | Detectar bandeja completamente insertada |
| RGB izquierdo | **D2-R** | Salida digital | Iluminación / estados visuales |
| RGB derecho | **D3-R** | Salida digital | Iluminación / estados visuales |
| OLED | Integrada | Pantalla | Estado técnico y diagnóstico |
| Buzzer | Integrado | Audio | Sonidos y alertas |

---

# 🔗 Distribución de los cuatro puertos I2C

```text
I2C1 ─── Sensor ToF izquierdo
I2C2 ─── LCD 16×2
I2C3 ─── LCD 16×4
I2C4 ─── Sensor ToF derecho
```

---

# 📷 Cámara M-Vision

La cámara **M-Vision** se comunica con el MATRIX Mini R4 mediante UART.

```text
M-Vision Cam
     │
     │ UART
     ▼
MATRIX Mini R4
```

## Función

La M-Vision realiza la detección visual de posibles objetos arqueológicos durante el harneado.

Sus funciones principales son:

- Capturar y analizar la zona interior del harnero.
- Analizar color y características visuales.
- Identificar objetos configurados en el sistema.
- Aplicar filtros para reducir falsas detecciones.
- Enviar al MATRIX el objeto detectado y su nivel de confianza.

Cuando se confirma un posible hallazgo, el MATRIX puede detener el movimiento, activar las alertas visuales y sonoras y comunicar el evento a **ATOM.IA**.

---

# 📏 Sensores láser ToF

ATOM ArchBot utiliza dos sensores ToF para medir la distancia entre los sensores y el material presente sobre el harnero.

## Sensor izquierdo

```text
Sensor ToF izquierdo
        │
        │ I2C1
        ▼
MATRIX Mini R4
```

## Sensor derecho

```text
Sensor ToF derecho
        │
        │ I2C4
        ▼
MATRIX Mini R4
```

## Función

Las mediciones permiten estimar la cantidad de tierra y modificar automáticamente el movimiento del harnero.

```text
Más tierra   → mayor velocidad de harneado
Menos tierra → menor velocidad de harneado
Hallazgo     → detener movimiento
```

---

# 🦾 Servomotores del harnero

ATOM ArchBot utiliza dos servomotores encargados de producir el movimiento del harnero.

```text
Servo izquierdo ───── SERVO 1 ─────┐
                                    │
                                    ▼
                             MATRIX Mini R4
                                    ▲
                                    │
Servo derecho ─────── SERVO 2 ─────┘
```

Los servos trabajan coordinados para generar el movimiento oscilante.

Cuando el sistema se detiene, entra en pausa o finaliza una acción, los servomotores pueden regresar a su posición inicial configurada.

---

# ⚙️ Motores DC de la bandeja

La bandeja inferior utiliza dos motores DC conectados a las salidas de motor del MATRIX Mini R4.

```text
Motor DC izquierdo ─── M1 / MOTOR 1 ───┐
                                        │
                                        ▼
                                 MATRIX Mini R4
                                        ▲
                                        │
Motor DC derecho ────── M2 / MOTOR 2 ───┘
```

## Función

Los motores permiten:

- Sacar la bandeja.
- Insertar la bandeja.
- Detener automáticamente la bandeja cuando se activa el final de carrera.

Actualmente:

```text
Pulsador 3 → Sacar bandeja
Pulsador 4 → Insertar bandeja
```

---

# 🖥️ Pantallas

ATOM ArchBot utiliza dos LCD externas y la OLED integrada del MATRIX.

## LCD 16×2 — I2C2

```text
LCD 16×2
   │
   │ I2C2
   ▼
MATRIX Mini R4
```

Se utiliza como interfaz principal para mostrar mensajes de funcionamiento, conexión e instrucciones.

---

## LCD 16×4 — I2C3

```text
LCD 16×4
   │
   │ I2C3
   ▼
MATRIX Mini R4
```

Puede mostrar información como:

```text
Tierra:     Baja
Velocidad:  ■■■□
Ubicación:  San Fernando
IA:         Conectado
```

Durante una detección también puede mostrar:

- Objeto detectado
- Confianza
- Estado del envío
- ID del objeto

---

## OLED integrada

La pantalla OLED forma parte del MATRIX Mini R4 y no requiere cableado externo.

Se utiliza para:

- Inicio del sistema.
- Logo de ATOM ArchBot.
- Logo de Los Harneritos.
- Estado de sensores.
- Diagnóstico.
- Errores.
- Información técnica.

---

# 🔘 Pulsadores externos

Los cuatro pulsadores están conectados en el **lado izquierdo del MATRIX Mini R4**.

| Pulsador | Puerto | Acción |
|---|---|---|
| Pulsador 1 | **D1-L** | Comenzar harneado |
| Pulsador 2 | **D2-L** | Pausar / continuar |
| Pulsador 3 | **D3-L** | Sacar bandeja |
| Pulsador 4 | **D4-L** | Insertar bandeja |

## Conexionado

Cada pulsador utiliza dos conexiones:

```text
Puerto digital ─── Pulsador ─── GND
```

Conexionado completo:

```text
D1-L ─── Pulsador 1 ─── GND
D2-L ─── Pulsador 2 ─── GND
D3-L ─── Pulsador 3 ─── GND
D4-L ─── Pulsador 4 ─── GND
```

---

# 🛑 Final de carrera

El final de carrera está conectado mediante **A1-L**.

## Conexionado

```text
Final de carrera

COM ───────── GND
NO  ───────── A1-L
```

La lógica utilizada es:

```text
Sin pulsar → HIGH
Pulsado    → LOW
```

Cuando la bandeja llega completamente al interior:

```text
Final de carrera activado
          ↓
A1-L pasa a LOW
          ↓
MATRIX detecta bandeja insertada
          ↓
Motores DC se detienen
```

Esto evita que los motores continúen empujando la bandeja después de llegar al límite mecánico.

---

# 🌈 Barras RGB WS2812B

ATOM ArchBot utiliza dos barras WS2812B de 8 LEDs.

Cada una posee una línea de datos independiente.

## RGB izquierdo — D2-R

```text
WS2812B izquierdo

DIN ───────── D2-R
5V  ───────── 5V
GND ───────── GND
```

## RGB derecho — D3-R

```text
WS2812B derecho

DIN ───────── D3-R
5V  ───────── 5V
GND ───────── GND
```

> La señal debe conectarse al pin **DIN** de la barra WS2812B, no a DOUT.

Al utilizar dos líneas independientes, ambas barras pueden controlarse por separado.

Esto permite:

- Animaciones simétricas.
- Indicadores de funcionamiento.
- Avisos de error.
- Confirmación de pulsadores.
- Iluminación durante el harneado.
- Señales visuales durante un hallazgo.
- Efectos del modo prueba/exhibición.

---

# 🔊 Buzzer integrado

El buzzer está integrado en el MATRIX Mini R4, por lo que no necesita cableado externo.

Se utiliza para:

- Encendido.
- Confirmación de pulsadores.
- Inicio del harneado.
- Cambio de estado.
- Movimiento de bandeja.
- Hallazgo.
- Errores.
- Comunicación con ATOM.IA.

---

# 🎛️ Botones integrados del MATRIX

Los botones integrados del MATRIX Mini R4 se utilizan para funciones auxiliares, diagnóstico y modos de prueba.

Al estar integrados directamente en el controlador, no requieren conexiones externas.

---

# ⚡ Sistema de alimentación

Actualmente ATOM ArchBot utiliza **4 portabaterías de 2 celdas**, para un total de:

### 🔋 8 baterías 18650

El sistema está configurado eléctricamente como:

```text
2S4P
```

Esto significa:

```text
4 celdas en paralelo → Grupo 1
4 celdas en paralelo → Grupo 2

Grupo 1 + Grupo 2 en serie
```

Valores aproximados:

```text
Voltaje nominal:       7,4 V
Voltaje máximo:        8,4 V
```

---

# 🛡️ BMS 2S

El sistema utiliza un **BMS 2S** para proteger las baterías.

El pack necesita tres puntos eléctricos principales:

```text
B- / 0 V ───── negativo del Grupo 1

B1 / BM ────── unión central entre
                Grupo 1 y Grupo 2

B+ / 8,4 V ─── positivo del Grupo 2
```

Representación:

```text
(-) PACK
   │
   ├────────────── B-
   │
[ GRUPO 1 ]
   │
   ├────────────── B1 / BM
   │
[ GRUPO 2 ]
   │
   ├────────────── B+
   │
(+) PACK
```

La salida protegida del BMS alimenta al robot.

```text
PACK 2S4P
   │
   ▼
BMS 2S
   │
   ▼
Interruptor general
   │
   ▼
Jack DC 5.5 × 2.1 mm
   │
   ▼
MATRIX Mini R4
```

---

# 🔌 Jack de alimentación

La salida del sistema de baterías llega al MATRIX Mini R4 mediante un conector:

### **DC 5.5 × 2.1 mm**

```text
BMS 2S
  │
  ▼
Interruptor
  │
  ▼
Jack DC 5.5 × 2.1 mm
  │
  ▼
MATRIX Mini R4
```

---

# 🗺️ Diagrama completo del conexionado

```text
                                      ┌─────────────────────┐
                                      │     M-Vision Cam    │
                                      └──────────┬──────────┘
                                                 │ UART
                                                 ▼

┌──────────────────┐ I2C1              ┌──────────────────────┐ I2C4 ┌──────────────────┐
│ ToF izquierdo    │───────────────────▶│                      │◀─────│ ToF derecho      │
└──────────────────┘                    │                      │      └──────────────────┘
                                        │                      │
┌──────────────────┐ I2C2              │                      │ I2C3 ┌──────────────────┐
│ LCD 16×2         │───────────────────▶│   MATRIX MINI R4     │◀─────│ LCD 16×4         │
└──────────────────┘                    │                      │      └──────────────────┘
                                        │                      │
D1-L ─ Pulsador 1 ─ GND ──────────────▶│                      │─────▶ SERVO 1
D2-L ─ Pulsador 2 ─ GND ──────────────▶│                      │─────▶ SERVO 2
D3-L ─ Pulsador 3 ─ GND ──────────────▶│                      │
D4-L ─ Pulsador 4 ─ GND ──────────────▶│                      │─────▶ M1 / Motor DC 1
                                        │                      │─────▶ M2 / Motor DC 2
A1-L ◀──── NO / Final carrera          │                      │
GND  ◀──── COM / Final carrera         │                      │─────▶ D2-R / RGB izquierdo
                                        │                      │─────▶ D3-R / RGB derecho
                                        │                      │
                                        │   OLED integrada     │
                                        │   Buzzer integrado   │
                                        └──────────────────────┘
```

---

# 🚦 Estados principales

| Estado | Comportamiento |
|---|---|
| `INICIALIZANDO` | Comprobación de componentes y posición inicial |
| `LISTO` | Sistema preparado para comenzar |
| `HARNEANDO` | Harnero activo y sensores funcionando |
| `PAUSADO` | Movimiento detenido temporalmente |
| `HALLAZGO` | Movimiento detenido y alertas activadas |
| `ERROR` | Sistema detenido por una condición de seguridad |
| `FINALIZADO` | Proceso terminado |
| `EXHIBICION / PRUEBA` | Funciones de demostración y diagnóstico |

---

# 🏺 ATOM ArchBot

**Sistema robótico de apoyo al harneado arqueológico**

El objetivo del sistema electrónico es integrar sensado, movimiento, visión artificial, control de bandeja, seguridad, información visual, alertas y comunicación con **ATOM.IA** en una única plataforma controlada mediante el **MATRIX Mini R4**.
