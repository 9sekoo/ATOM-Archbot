# ⚙️ Funcionamiento de ATOM ArchBot

Este documento describe la **lógica de funcionamiento propuesta** para **ATOM ArchBot**, desde su encendido hasta la finalización del proceso de harneado.

> [!IMPORTANT]
> El funcionamiento descrito en este documento corresponde a una **propuesta de diseño y programación**.
>
> Durante la integración y programación real de ATOM ArchBot, algunos comportamientos, tiempos, velocidades, estados o secuencias pueden ser modificados según los resultados obtenidos en las pruebas del prototipo.

---

# 🏺 ¿Qué hace ATOM ArchBot?

**ATOM ArchBot** es un sistema robótico diseñado para apoyar el proceso de **harneado arqueológico**.

Su funcionamiento combina:

* 🤖 Movimiento automatizado del harnero.
* 📏 Medición de material mediante sensores láser.
* 📷 Visión artificial mediante la M-Vision Cam.
* 🧠 Procesamiento mediante el MATRIX Mini R4.
* ⚙️ Movimiento automatizado de la bandeja.
* 🖥️ Información mediante pantallas.
* 🌈 Alertas visuales RGB.
* 🔊 Alertas mediante buzzer.
* 🛑 Sistemas de seguridad y autodiagnóstico.

La idea principal es que el robot pueda **adaptar el movimiento del harnero dependiendo de la cantidad de material existente y reaccionar ante la detección de un posible objeto arqueológico**.

---

# 🔄 Flujo general de funcionamiento

```text
┌───────────────────────────┐
│      ENCENDER ATOM        │
└─────────────┬─────────────┘
              │
              ▼
┌───────────────────────────┐
│      INICIALIZACIÓN       │
│                           │
│ • MATRIX Mini R4          │
│ • M-Vision Cam            │
│ • Sensores láser          │
│ • Servomotores            │
│ • Motores DC              │
│ • Pantallas               │
│ • RGB                     │
│ • Final de carrera        │
└─────────────┬─────────────┘
              │
              ▼
┌───────────────────────────┐
│      AUTODIAGNÓSTICO      │
└─────────────┬─────────────┘
              │
              ▼
        ¿TODO CORRECTO?
           /       \
         NO         SÍ
         │           │
         ▼           ▼
┌──────────────┐  ┌─────────────────┐
│    ERROR     │  │      LISTO      │
│              │  │                 │
│ No comenzar  │  │ Esperar inicio  │
└──────────────┘  └────────┬────────┘
                           │
                           ▼
                  Pulsador INICIAR
                           │
                           ▼
                  ┌─────────────────┐
                  │    HARNEANDO    │
                  └────────┬────────┘
                           │
              ┌────────────┴────────────┐
              │                         │
              ▼                         ▼
      Sensores láser             M-Vision Cam
      miden material             analiza imagen
              │                         │
              ▼                         ▼
      Ajustar velocidad         Buscar posible
       de los servos              hallazgo
              │                         │
              └────────────┬────────────┘
                           │
                           ▼
                  ¿HAY HALLAZGO?
                     /       \
                   SÍ         NO
                   │           │
                   ▼           │
            ┌──────────────┐   │
            │   HALLAZGO   │   │
            │              │   │
            │ Detener      │   │
            │ harnero      │   │
            │ RGB + buzzer │   │
            └──────────────┘   │
                               │
                               ▼
                       Continuar proceso
                               │
                               ▼
                      ¿HARNEADO TERMINÓ?
                          /        \
                        NO          SÍ
                        │            │
                        └──────┐     ▼
                               │ ┌──────────────┐
                               └▶│ FINALIZADO   │
                                 └──────────────┘
```

---

# 🚀 1. Encendido

El funcionamiento comienza cuando el usuario activa el **interruptor general de ATOM ArchBot**.

```text
Interruptor
    ↓
Alimentación
    ↓
MATRIX Mini R4
    ↓
Inicio del sistema
```

Una vez energizado, el MATRIX Mini R4 comienza la secuencia de inicialización.

Como señal de encendido se podrá utilizar:

* 🔊 Melodía de inicio mediante el buzzer.
* 🌈 Animación breve de las luces RGB.
* 🖥️ Mensaje de inicio en las pantallas.
* ⚙️ Posicionamiento inicial de los actuadores.

Ejemplo:

```text
┌────────────────┐
│   ATOM ARCHBOT │
│   INICIANDO... │
└────────────────┘
```

---

# 🧠 2. Inicialización del sistema

Antes de permitir el harneado, ATOM debe preparar sus principales subsistemas.

Durante esta etapa se inicializan:

```text
MATRIX Mini R4
      │
      ├── M-Vision Cam
      ├── Sensor láser izquierdo
      ├── Sensor láser derecho
      ├── Servo izquierdo
      ├── Servo derecho
      ├── Motores DC
      ├── LCD 16x2
      ├── LCD 16x4
      ├── OLED integrada
      ├── RGB izquierdo
      ├── RGB derecho
      └── Final de carrera
```

Los servomotores se llevan a una posición inicial conocida.

```text
Servo izquierdo → 0°
Servo derecho   → 0°
```

Esto permite comenzar cada ciclo desde una posición controlada.

---

# 🩺 3. Autodiagnóstico

Una vez inicializados los componentes, el sistema realiza un **autodiagnóstico básico** antes de permitir el inicio del harneado.

El objetivo es detectar posibles fallos antes de mover el mecanismo.

El sistema podrá comprobar:

| Componente             | Comprobación propuesta  |
| ---------------------- | ----------------------- |
| M-Vision Cam           | Comunicación disponible |
| Sensor láser izquierdo | Lectura válida          |
| Sensor láser derecho   | Lectura válida          |
| Servomotores           | Posición inicial        |
| Pantallas              | Inicialización correcta |
| Final de carrera       | Estado lógico           |
| RGB                    | Inicialización          |
| Sistemas principales   | Comunicación y estado   |

---

## ✅ Diagnóstico correcto

Si los componentes principales responden correctamente:

```text
AUTODIAGNÓSTICO
       ↓
     TODO OK
       ↓
      LISTO
```

El sistema queda preparado para iniciar.

---

## ❌ Error detectado

Si se detecta un problema:

```text
AUTODIAGNÓSTICO
       ↓
    ERROR
       ↓
NO INICIAR HARNEADO
```

La pantalla puede indicar específicamente el componente afectado.

Ejemplo:

```text
ERROR SISTEMA
CAMARA NO DETECTADA
```

o:

```text
ERROR SISTEMA
LASER IZQ.
```

El sistema deberá permanecer detenido hasta solucionar el problema o reiniciar el robot.

---

# ✅ 4. Estado LISTO

Cuando la inicialización y el autodiagnóstico terminan correctamente, ATOM pasa al estado:

```text
LISTO
```

En este estado:

* Los servomotores permanecen detenidos.
* Los motores de la bandeja permanecen detenidos.
* La cámara puede permanecer preparada.
* Los sensores se encuentran disponibles.
* Las pantallas informan que el sistema puede comenzar.
* El robot espera una acción del usuario.

La pantalla principal podría mostrar:

```text
┌────────────────┐
│ ATOM LISTO     │
│ ¿INICIAR?      │
└────────────────┘
```

El usuario inicia el proceso mediante el:

### 🔘 Pulsador 1 — INICIAR

---

# ⚙️ 5. Inicio del harneado

Al presionar el botón de inicio:

```text
Pulsador 1
    ↓
MATRIX Mini R4
    ↓
Estado HARNEANDO
```

Los dos servomotores comienzan a realizar el movimiento oscilante necesario para mover el harnero.

```text
Servo izquierdo  ↔
                   HARNEO
Servo derecho    ↔
```

Los dos actuadores deberán trabajar de forma coordinada para mantener un movimiento estable.

---

# 📏 6. Medición del material

Mientras ATOM está harneando, los **dos sensores láser** realizan mediciones sobre el contenido presente en el harnero.

```text
       SENSOR IZQ.        SENSOR DER.
            ↓                  ↓
        ┌─────────────────────────┐
        │        MATERIAL         │
        │       DEL HARNERO       │
        └─────────────────────────┘
```

Estas mediciones permiten obtener información aproximada sobre la distancia entre los sensores y el material.

---

# 🧠 7. Control adaptativo del movimiento

Uno de los objetivos del sistema es que la velocidad del harnero **no sea siempre la misma**.

ATOM podrá modificar la intensidad o velocidad del movimiento dependiendo de las lecturas obtenidas.

La lógica propuesta es:

```text
MUCHA TIERRA
     ↓
Material más cerca de los sensores
     ↓
Mayor velocidad de harneado
```

Mientras que:

```text
POCA TIERRA
     ↓
Material más lejos de los sensores
     ↓
Menor velocidad de harneado
```

Esto busca conseguir un proceso más controlado y reducir movimientos innecesariamente fuertes cuando quede poco material.

---

## ⚡ Ejemplo conceptual

| Situación           | Lectura aproximada   | Movimiento      |
| ------------------- | -------------------- | --------------- |
| Mucho material      | Distancia pequeña    | Más rápido      |
| Material intermedio | Distancia media      | Velocidad media |
| Poco material       | Distancia mayor      | Más lento       |
| Posible hallazgo    | Detectado por visión | Detener         |

> Los valores exactos de distancia, velocidad y ángulos se determinarán experimentalmente durante las pruebas del robot.

---

# 📷 8. Visión artificial

Mientras se realiza el harneado, la **M-Vision Cam** observa el interior del sistema.

```text
Harnero
   ↓
M-Vision Cam
   ↓
Procesamiento de imagen
   ↓
Análisis
```

La cámara puede utilizar características como:

* Color.
* Forma.
* Tamaño.
* Área.
* Cantidad de píxeles.
* Densidad.
* Posición dentro de la imagen.
* Persistencia durante varios fotogramas.

El objetivo no es reaccionar inmediatamente ante cualquier color extraño, sino utilizar diferentes filtros para reducir falsos positivos.

---

# 🔍 9. Detección de un posible hallazgo

Cuando la cámara encuentra un objeto que cumple con los criterios definidos, el sistema puede realizar una comprobación durante varios fotogramas.

```text
OBJETO DETECTADO
       ↓
¿APARECE DURANTE VARIOS FRAMES?
       ↓
      SÍ
       ↓
POSIBLE HALLAZGO
```

Esto permite evitar que pequeñas sombras, movimientos de tierra o reflejos generen una alerta inmediatamente.

---

# 🏺 10. Estado HALLAZGO

Cuando se confirma un posible objeto de interés:

```text
M-Vision Cam
      ↓
Posible objeto detectado
      ↓
MATRIX Mini R4
```

ATOM pasa al estado:

```text
HALLAZGO
```

En este estado se propone realizar automáticamente:

### 🛑 Detención del harnero

```text
Servo izquierdo → 0°
Servo derecho   → 0°
```

### 🌈 Alerta visual

Las luces RGB comienzan una animación o parpadeo de advertencia.

```text
RGB IZQUIERDO  ✨ ✨ ✨
RGB DERECHO    ✨ ✨ ✨
```

### 🔊 Alerta sonora

El buzzer reproduce la melodía correspondiente a un hallazgo.

### 🖥️ Aviso en pantalla

Ejemplo:

```text
┌────────────────┐
│ POSIBLE        │
│ HALLAZGO       │
└────────────────┘
```

El objetivo es que una persona pueda revisar físicamente el objeto antes de continuar el proceso.

---

# ⏸️ 11. Pausa y continuación

Durante el funcionamiento, el usuario puede utilizar:

### 🔘 Pulsador 2 — PAUSA / CONTINUAR

Al presionarlo durante el harneado:

```text
HARNEANDO
    ↓
Pulsador 2
    ↓
PAUSADO
```

Durante la pausa:

* Se detiene el movimiento del harnero.
* Los servomotores quedan en una posición segura.
* El sistema mantiene sus datos.
* Las pantallas indican que el proceso está pausado.

Ejemplo:

```text
┌────────────────┐
│    PAUSADO     │
│ PULSE CONTINUAR│
└────────────────┘
```

Al volver a pulsarlo:

```text
PAUSADO
   ↓
Pulsador 2
   ↓
HARNEANDO
```

---

# 📦 12. Control de la bandeja

ATOM ArchBot posee una bandeja inferior que puede entrar y salir mediante dos motores DC.

---

## 📤 Sacar bandeja

El:

### 🔘 Pulsador 3

activa el movimiento de extracción.

```text
Pulsador 3
    ↓
Motores DC
    ↓
SACAR BANDEJA
```

---

## 📥 Insertar bandeja

El:

### 🔘 Pulsador 4

activa el movimiento de inserción.

```text
Pulsador 4
    ↓
Motores DC
    ↓
INSERTAR BANDEJA
```

---

# 🛑 13. Final de carrera

Durante la inserción de la bandeja existe un **final de carrera** que permite saber cuándo ésta ha llegado a su posición final.

```text
Bandeja entra
     ↓
Activa final de carrera
     ↓
MATRIX recibe señal
     ↓
Detener motores DC
```

Esto permite evitar que los motores continúen empujando la bandeja después de alcanzar su límite mecánico.

---

# 🏁 14. Finalización del harneado

Cuando el sistema determine que el proceso ha terminado, ATOM pasa al estado:

```text
FINALIZADO
```

En esta condición:

* Los servomotores se detienen.
* Los motores DC quedan apagados.
* La cámara deja de buscar durante el ciclo actual o queda en espera.
* Las luces RGB muestran una señal de finalización.
* El buzzer puede reproducir una melodía.
* Las pantallas informan al usuario.

Ejemplo:

```text
┌────────────────┐
│   HARNEADO     │
│   FINALIZADO   │
└────────────────┘
```

---

# 🚦 Estados principales del sistema

La lógica de ATOM ArchBot se puede representar mediante diferentes estados.

| Estado            | Descripción                            |
| ----------------- | -------------------------------------- |
| `INICIALIZANDO`   | Preparación de hardware y subsistemas  |
| `AUTODIAGNOSTICO` | Comprobación inicial de componentes    |
| `LISTO`           | Robot preparado para comenzar          |
| `HARNEANDO`       | Harnero funcionando normalmente        |
| `PAUSADO`         | Proceso detenido temporalmente         |
| `HALLAZGO`        | Posible objeto detectado               |
| `ERROR`           | Problema detectado en algún subsistema |
| `FINALIZADO`      | Ciclo de harneado terminado            |

---

# 🧩 Diagrama simplificado de estados

```text
                    ┌───────────────┐
                    │    ENCENDIDO  │
                    └───────┬───────┘
                            │
                            ▼
                    ┌───────────────┐
                    │ INICIALIZANDO │
                    └───────┬───────┘
                            │
                            ▼
                  ┌──────────────────┐
                  │ AUTODIAGNÓSTICO  │
                  └────────┬─────────┘
                           │
                ┌──────────┴──────────┐
                │                     │
              ERROR                   OK
                │                     │
                ▼                     ▼
        ┌───────────────┐      ┌───────────────┐
        │     ERROR     │      │     LISTO     │
        └───────────────┘      └───────┬───────┘
                                       │
                                    INICIAR
                                       │
                                       ▼
                               ┌───────────────┐
                         ┌────▶│   HARNEANDO   │◀────┐
                         │     └───────┬───────┘     │
                         │             │             │
                    CONTINUAR       PAUSAR         NO
                         │             │             │
                         │             ▼             │
                         │     ┌───────────────┐     │
                         └─────│    PAUSADO    │     │
                               └───────────────┘     │
                                       │             │
                               POSIBLE HALLAZGO      │
                                       │             │
                                       ▼             │
                               ┌───────────────┐     │
                               │   HALLAZGO    │     │
                               └───────────────┘     │
                                                     │
                               ¿PROCESO TERMINÓ? ────┘
                                       │
                                      SÍ
                                       │
                                       ▼
                               ┌───────────────┐
                               │  FINALIZADO   │
                               └───────────────┘
```

---

# 🧠 Lógica conceptual

De manera simplificada, el programa principal podría seguir la siguiente lógica:

```text
ENCENDER

↓
Inicializar componentes

↓
Ejecutar autodiagnóstico

↓
SI existe un error:
    Mostrar error
    Bloquear inicio

SI todo está correcto:
    Estado = LISTO

↓
Esperar botón INICIAR

↓
Estado = HARNEANDO

↓
Mientras se realiza el harneado:

    Leer sensor láser izquierdo
    Leer sensor láser derecho

    Calcular nivel aproximado de material

    Ajustar velocidad de servos

    Analizar imagen de M-Vision

    SI existe posible hallazgo:
        Detener servos
        Activar RGB
        Activar buzzer
        Mostrar alerta
        Estado = HALLAZGO

    SI usuario pulsa PAUSA:
        Detener movimiento
        Estado = PAUSADO

    SI usuario solicita bandeja:
        Controlar motores DC

    SI final de carrera se activa:
        Detener inserción

    SI proceso termina:
        Detener actuadores
        Estado = FINALIZADO
```

---

# 🛡️ Principios de funcionamiento

El diseño de la programación de ATOM ArchBot buscará priorizar:

### 🏺 Protección del material

El robot debe evitar movimientos innecesariamente fuertes cuando exista poco material o se detecte un posible objeto.

### 🛑 Seguridad mecánica

Los actuadores deben detenerse ante estados de error, pausa o condiciones que puedan producir un funcionamiento incorrecto.

### 👁️ Confirmación antes de actuar

La visión artificial utilizará diferentes criterios antes de considerar un objeto como posible hallazgo.

### 🧠 Funcionamiento por estados

La programación se organizará mediante estados claramente definidos para evitar acciones contradictorias.

### 👤 Supervisión humana

ATOM ArchBot funciona como una herramienta de **apoyo** al trabajo arqueológico.

Una detección realizada por el sistema debe considerarse un **posible hallazgo**, cuya identificación final corresponde a una persona capacitada.

---

# 🔬 Desarrollo experimental

Gran parte del comportamiento definitivo será determinado durante las pruebas del prototipo.

Será necesario experimentar con:

* Distancias de los sensores láser.
* Velocidad de los servomotores.
* Ángulos de movimiento.
* Cantidad de tierra.
* Sensibilidad de la cámara.
* Condiciones de iluminación.
* Umbrales de detección.
* Número de fotogramas necesarios para confirmar objetos.
* Tiempo de respuesta de las alertas.
* Comportamiento de la bandeja.
* Posición del final de carrera.

Los resultados obtenidos permitirán ajustar progresivamente el software.

---

# ⚠️ Estado actual del diseño

> [!NOTE]
> Esta documentación representa el **funcionamiento previsto de ATOM ArchBot antes de completar la programación final**.
>
> La arquitectura general del sistema está definida, pero los parámetros específicos y algunas decisiones de software podrán cambiar durante la integración, calibración y validación experimental.
>
> Estos cambios serán documentados a medida que avance el desarrollo.

---

# 🏺 ATOM ArchBot

### Sistema robótico de apoyo al harneado arqueológico

```text
SENSAR
   ↓
ANALIZAR
   ↓
ADAPTAR
   ↓
PROTEGER
```

El objetivo de ATOM ArchBot es utilizar la robótica y la visión artificial para transformar el harneado en un proceso más **controlado, adaptable y seguro para la conservación de posibles restos arqueológicos**.
