# 🔌 Conexiones de ATOM ArchBot

Este documento describe la distribución de conexiones eléctricas y de comunicación entre el **MATRIX Mini R4** y los distintos componentes electrónicos de **ATOM ArchBot**.

> **Nota:** La distribución corresponde al esquema actual del prototipo V3 y puede recibir pequeños ajustes durante las pruebas finales de integración.

---

## 🧠 Controlador principal

El controlador central de ATOM ArchBot es el:

### **MATRIX Mini R4**

El MATRIX Mini R4 se encarga de recibir la información de los sensores, procesar los estados del sistema y controlar los distintos actuadores.

Desde este controlador se conectan:

* Cámara **M-Vision**
* 2 sensores láser
* 2 servomotores
* 2 motores DC
* 4 pulsadores externos
* Final de carrera
* Pantalla LCD 16x2
* Pantalla LCD 16x4
* 2 módulos RGB WS2812B
* OLED integrada
* Buzzer integrado
* Botones integrados del MATRIX

---

# 📋 Resumen general de conexiones

## Sensores y entradas

| Componente             | Puerto / conexión | Tipo             | Función                                     |
| ---------------------- | ----------------- | ---------------- | ------------------------------------------- |
| M-Vision Cam           | UART              | Comunicación     | Sistema de visión artificial                |
| Sensor láser izquierdo | I2C 1             | Entrada / sensor | Medición del nivel y distancia del material |
| Sensor láser derecho   | I2C 2             | Entrada / sensor | Medición del nivel y distancia del material |
| Pulsador 1             | Entrada digital   | Entrada          | Comenzar harneado                           |
| Pulsador 2             | Entrada digital   | Entrada          | Pausar / continuar                          |
| Pulsador 3             | Entrada digital   | Entrada          | Sacar bandeja                               |
| Pulsador 4             | Entrada digital   | Entrada          | Insertar bandeja                            |
| Final de carrera       | Entrada digital   | Entrada          | Detectar posición final de la bandeja       |
| Botones MATRIX         | Integrados        | Entrada          | Controles auxiliares y pruebas              |

---

## ⚙️ Actuadores y salidas

| Componente         | Puerto / conexión | Tipo              | Función                            |
| ------------------ | ----------------- | ----------------- | ---------------------------------- |
| Servo izquierdo    | SERVO 1           | Salida            | Movimiento del harnero             |
| Servo derecho      | SERVO 2           | Salida            | Movimiento del harnero             |
| Motor DC izquierdo | MOTOR 1           | Salida            | Movimiento de la bandeja           |
| Motor DC derecho   | MOTOR 2           | Salida            | Movimiento de la bandeja           |
| LCD 16x2           | I2C 3             | Salida / pantalla | Interfaz principal para el usuario |
| LCD 16x4           | I2C 4             | Salida / pantalla | Información detallada del sistema  |
| RGB izquierdo      | Puerto derecho 1  | Salida            | Iluminación e indicador visual     |
| RGB derecho        | Puerto derecho 2  | Salida            | Iluminación e indicador visual     |
| OLED MATRIX        | Integrada         | Salida / pantalla | Estado técnico y diagnóstico       |
| Buzzer MATRIX      | Integrado         | Salida / audio    | Alertas y sonidos del sistema      |

---

# 📷 Cámara M-Vision

## Conexión

La cámara **M-Vision** se comunica con el MATRIX Mini R4 mediante:

```text
M-Vision Cam
     │
     │ UART
     ▼
MATRIX Mini R4
```

## Función

La cámara se encarga del sistema de **visión artificial** de ATOM ArchBot.

Entre sus funciones se encuentran:

* Capturar imágenes del interior del harnero.
* Analizar color y forma.
* Identificar posibles objetos arqueológicos.
* Evitar detecciones falsas mediante filtros.
* Informar al MATRIX cuando exista un posible hallazgo.

Cuando la cámara confirma un objeto de interés, envía una señal al MATRIX para que el sistema pueda detener el movimiento del harnero y activar las alertas correspondientes.

---

# 📏 Sensores láser

ATOM ArchBot utiliza **dos sensores láser**, uno orientado hacia cada zona del harnero.

## Sensor izquierdo

```text
Sensor láser izquierdo
        │
        │ I2C 1
        ▼
MATRIX Mini R4
```

## Sensor derecho

```text
Sensor láser derecho
        │
        │ I2C 2
        ▼
MATRIX Mini R4
```

## Función

Los sensores permiten estimar la distancia entre ellos y el material presente sobre el harnero.

Esta información podrá utilizarse para modificar automáticamente el movimiento de los servomotores:

* **Mayor cantidad de tierra → mayor velocidad de harneado**
* **Menor cantidad de tierra → menor velocidad de harneado**
* **Posible objeto detectado → detención del movimiento**

---

# 🦾 Servomotores del harnero

ATOM ArchBot utiliza **dos servomotores** encargados de generar el movimiento oscilante del harnero.

## Conexiones

```text
Servo izquierdo ───── SERVO 1 ─────┐
                                    │
                                    ▼
                            MATRIX Mini R4
                                    ▲
                                    │
Servo derecho ─────── SERVO 2 ─────┘
```

Los dos servomotores trabajan de forma coordinada.

Cuando el sistema se encuentra en reposo, inicialización, pausa, error o detección de un posible objeto arqueológico, los servomotores pueden regresar a su posición de referencia de **0°**.

---

# ⚙️ Motores DC de la bandeja

La bandeja inferior utiliza **dos motores DC**.

## Conexiones

```text
Motor DC izquierdo ─── MOTOR 1 ───┐
                                   │
                                   ▼
                           MATRIX Mini R4
                                   ▲
                                   │
Motor DC derecho ───── MOTOR 2 ───┘
```

## Función

Los motores permiten:

* Insertar la bandeja.
* Sacar la bandeja.
* Detener automáticamente su inserción cuando se activa el final de carrera.

---

# 🖥️ Pantallas

ATOM ArchBot utiliza tres interfaces de visualización.

## LCD 16x2

```text
LCD 16x2
   │
   │ I2C 3
   ▼
MATRIX Mini R4
```

Se utiliza como **interfaz principal con el usuario**.

Puede mostrar mensajes como:

```text
ATOM ARCHBOT
¿INICIAR?
```

---

## LCD 16x4

```text
LCD 16x4
   │
   │ I2C 4
   ▼
MATRIX Mini R4
```

Permite mostrar información más detallada del proceso, sensores y estados del sistema.

---

## OLED integrada

La pantalla OLED integrada en el MATRIX Mini R4 se utiliza principalmente para información técnica.

Puede mostrar:

* Estado de la cámara.
* Estado de sensores.
* Estado de actuadores.
* Proceso de inicialización.
* Autodiagnóstico.
* Códigos de error.
* Estado general del sistema.

---

# 🔘 Pulsadores externos

El sistema utiliza cuatro pulsadores físicos.

| Pulsador   | Acción             |
| ---------- | ------------------ |
| Pulsador 1 | Comenzar harneado  |
| Pulsador 2 | Pausar / continuar |
| Pulsador 3 | Sacar bandeja      |
| Pulsador 4 | Insertar bandeja   |

Cada pulsador se conecta a una **entrada digital** del MATRIX Mini R4 junto con su referencia eléctrica correspondiente.

---

# 🛑 Final de carrera

El final de carrera permite detectar cuándo la bandeja ha llegado completamente a su posición de inserción.

```text
Final de carrera
      │
      │ Entrada digital
      ▼
MATRIX Mini R4
```

Cuando se activa:

```text
Final de carrera activado
          ↓
MATRIX recibe la señal
          ↓
Motores DC se detienen
```

Esto evita que los motores continúen intentando mover la bandeja una vez alcanzado el límite mecánico.

---

# 🌈 Sistema RGB

ATOM ArchBot utiliza **dos módulos RGB WS2812B de 8 LEDs**.

Los módulos se controlarán **por separado** para disponer de mayor libertad en las animaciones y estados visuales.

## RGB izquierdo

```text
WS2812B izquierdo
      │
      │ DATA
      ▼
Puerto derecho 1
      │
      ▼
MATRIX Mini R4
```

Además necesita:

```text
5V
GND
DATA
```

---

## RGB derecho

```text
WS2812B derecho
      │
      │ DATA
      ▼
Puerto derecho 2
      │
      ▼
MATRIX Mini R4
```

También utiliza:

```text
5V
GND
DATA
```

Al tener líneas de datos independientes, ambos lados pueden controlarse individualmente.

Por ejemplo:

```text
RGB izquierdo        RGB derecho
      │                    │
      └────── MATRIX ──────┘
```

Esto permite realizar:

* Encendido independiente.
* Parpadeos de alerta.
* Animaciones simétricas.
* Indicadores de estado.
* Iluminación del interior.
* Señales visuales durante un hallazgo.

---

# 🔊 Buzzer integrado

El MATRIX Mini R4 incorpora un buzzer que se utilizará como sistema de señalización acústica.

No requiere un módulo externo adicional.

Entre sus funciones estarán:

* Sonido de encendido.
* Confirmación de inicio.
* Aviso de posible hallazgo.
* Cambio de velocidad.
* Alertas de error.
* Finalización del harneado.

---

# ⚡ Sistema de alimentación

La alimentación se mantiene separada de las conexiones de control y comunicación para facilitar la comprensión del sistema.

ATOM ArchBot utilizará un total de:

### 🔋 6 baterías

Las seis baterías estarán organizadas formando un pack:

```text
2S3P
```

Esto significa:

* **2 grupos en serie**
* **3 baterías en paralelo por grupo**
* **6 baterías en total**

El pack tendrá aproximadamente:

```text
Voltaje nominal: 7,4 V
Voltaje máximo cargado: 8,4 V
```

---

# 🛡️ BMS

El pack de baterías estará conectado a un:

### **BMS 2S**

El BMS se encarga de proteger y administrar eléctricamente el conjunto de baterías.

El flujo general será:

```text
┌──────────────────────┐
│     6 BATERÍAS       │
│        2S3P          │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│       BMS 2S         │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│ INTERRUPTOR GENERAL  │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│   JACK DC 5.5x2.1    │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│   MATRIX MINI R4     │
└──────────────────────┘
```

Por lo tanto, la ruta principal de alimentación será:

```text
6 baterías → BMS 2S → interruptor → Jack DC → MATRIX Mini R4
```

---

# 🔌 Jack de alimentación

La salida proveniente del sistema de baterías y del BMS llegará al MATRIX Mini R4 mediante un conector:

### **Jack DC 5.5 × 2.1 mm**

```text
BMS
 │
 ▼
Interruptor
 │
 ▼
Jack DC
 │
 ▼
MATRIX Mini R4
```

Esto permite mantener una conexión de alimentación firme y fácilmente desconectable.

---

# 🔄 Flujo general del sistema

```text
                    ┌─────────────────┐
                    │  M-Vision Cam   │
                    └────────┬────────┘
                             │ UART
                             ▼
┌──────────────┐      ┌───────────────┐      ┌────────────────┐
│ Láser izq.   │─────▶│               │◀─────│ Láser derecho  │
└──────────────┘ I2C  │ MATRIX Mini R4│ I2C  └────────────────┘
                      │               │
┌──────────────┐─────▶│               │─────▶ Servo izquierdo
│ Pulsadores   │      │               │─────▶ Servo derecho
└──────────────┘      │               │
                      │               │─────▶ Motor DC izquierdo
┌──────────────┐─────▶│               │─────▶ Motor DC derecho
│ Final carrera│      │               │
└──────────────┘      │               │─────▶ LCD 16x2
                      │               │─────▶ LCD 16x4
                      │               │─────▶ RGB izquierdo
                      │               │─────▶ RGB derecho
                      │               │─────▶ OLED
                      │               │─────▶ Buzzer
                      └───────────────┘
```

---

# 🚦 Estados principales

Las conexiones anteriores permiten implementar los estados principales de funcionamiento de ATOM ArchBot:

| Estado          | Comportamiento                                 |
| --------------- | ---------------------------------------------- |
| `INICIALIZANDO` | Comprobación de componentes y posición inicial |
| `LISTO`         | Sistema preparado para comenzar                |
| `HARNEANDO`     | Harnero en movimiento y sensores activos       |
| `PAUSADO`       | Movimiento detenido temporalmente              |
| `HALLAZGO`      | Movimiento detenido y alertas activadas        |
| `ERROR`         | Sistema bloqueado hasta solucionar el problema |
| `FINALIZADO`    | Proceso de harneado completado                 |

---

## 🏺 ATOM ArchBot

**Sistema robótico de apoyo al harneado arqueológico**

El objetivo del sistema electrónico es integrar sensado, movimiento, visión artificial y alertas en una única plataforma controlada mediante el **MATRIX Mini R4**.
