🔌 Conexionado real de ATOM ArchBot V3
Este documento describe el conexionado físico real utilizado en ATOM ArchBot V3 con el MATRIX Mini R4. La distribución indicada aquí reemplaza los esquemas genéricos anteriores y sigue el montaje probado del robot.
> **Importante:** Los nombres `-L` y `-R` indican los conectores ubicados en el lado izquierdo y derecho del MATRIX Mini R4, respectivamente.
---
🧠 Controlador principal
El controlador central del sistema es el:
MATRIX Mini R4
El MATRIX Mini R4 recibe las señales de los sensores y pulsadores, controla los actuadores y administra las interfaces visuales y sonoras del robot.
Componentes conectados:
Cámara M-Vision
2 sensores láser ToF
2 servomotores del harnero
2 motores DC de la bandeja
4 pulsadores externos
1 final de carrera
LCD 16×2
LCD 16×4
2 barras RGB WS2812B de 8 LEDs
OLED integrada
Buzzer integrado
Botones integrados del MATRIX
---
📋 Mapa general de conexiones
Componente	Puerto real	Tipo	Función
M-Vision Cam	UART / puerto M-Vision	Comunicación	Visión artificial
Sensor ToF izquierdo	I2C1	Sensor	Medición de distancia / cantidad de tierra
LCD 16×2	I2C2	Pantalla	Mensajes principales al usuario
LCD 16×4	I2C3	Pantalla	Datos detallados del proceso
Sensor ToF derecho	I2C4	Sensor	Medición de distancia / cantidad de tierra
Servo izquierdo	SERVO 1	Actuador	Movimiento del harnero
Servo derecho	SERVO 2	Actuador	Movimiento del harnero
Motor DC izquierdo	M1 / MOTOR 1	Actuador	Movimiento de bandeja
Motor DC derecho	M2 / MOTOR 2	Actuador	Movimiento de bandeja
Pulsador 1	D1-L	Entrada digital	Comenzar harneado
Pulsador 2	D2-L	Entrada digital	Pausar / continuar
Pulsador 3	D3-L	Entrada digital	Sacar bandeja
Pulsador 4	D4-L	Entrada digital	Insertar bandeja
Final de carrera	A1-L	Entrada digital	Detectar bandeja completamente insertada
RGB izquierdo	D2-R	Salida digital	Iluminación / estados visuales
RGB derecho	D3-R	Salida digital	Iluminación / estados visuales
OLED	Integrada	Pantalla	Estado técnico y diagnóstico
Buzzer	Integrado	Audio	Sonidos y alertas
---
🔗 Distribución de los cuatro puertos I2C
Una parte importante del montaje real es que los cuatro dispositivos I2C están distribuidos de esta forma:
```text
I2C1 ─── Sensor ToF izquierdo
I2C2 ─── LCD 16×2
I2C3 ─── LCD 16×4
I2C4 ─── Sensor ToF derecho
```
De esta forma cada dispositivo utiliza el puerto físico que corresponde al montaje del ATOM ArchBot.
---
📷 Cámara M-Vision
La cámara M-Vision se comunica con el MATRIX Mini R4 mediante la interfaz de visión/UART del controlador.
```text
M-Vision Cam
     │
     │ UART
     ▼
MATRIX Mini R4
```
Función
La M-Vision realiza la detección visual de posibles objetos arqueológicos durante el harneado.
Entre sus funciones se encuentran:
Capturar y analizar la zona interior del harnero.
Analizar color y características visuales.
Identificar objetos configurados en el sistema de visión.
Aplicar filtros para reducir falsas detecciones.
Enviar al MATRIX el objeto detectado y su nivel de confianza.
Cuando se confirma un hallazgo válido, el MATRIX puede detener el movimiento, activar las alertas visuales y sonoras y comunicar el evento a ATOM.IA.
---
📏 Sensores láser ToF
ATOM ArchBot utiliza dos sensores ToF para estimar la distancia entre el sensor y el material presente sobre el harnero.
Sensor izquierdo
```text
Sensor ToF izquierdo
        │
        │ I2C1
        ▼
MATRIX Mini R4
```
Sensor derecho
```text
Sensor ToF derecho
        │
        │ I2C4
        ▼
MATRIX Mini R4
```
Función
Las mediciones se utilizan para estimar la cantidad de tierra y adaptar el movimiento del harnero.
```text
Más tierra   → mayor velocidad de harneado
Menos tierra → menor velocidad de harneado
Hallazgo     → detener movimiento
```
Los sensores también permiten aplicar bloqueos de seguridad relacionados con la presencia de tierra y la posición de la bandeja.
---
🦾 Servomotores del harnero
ATOM ArchBot utiliza dos servomotores encargados de generar el movimiento oscilante del harnero.
```text
Servo izquierdo ───── SERVO 1 ─────┐
                                    │
                                    ▼
                             MATRIX Mini R4
                                    ▲
                                    │
Servo derecho ─────── SERVO 2 ─────┘
```
Los servos trabajan coordinados y pueden utilizar movimientos opuestos para generar la oscilación del harnero.
Al finalizar una acción, entrar en pausa, producirse un error o detenerse el proceso, el firmware puede devolver ambos servos a su posición inicial configurada.
---
⚙️ Motores DC de la bandeja
La bandeja inferior es desplazada mediante dos motores DC conectados directamente a las salidas de motor del MATRIX Mini R4.
```text
Motor DC izquierdo ─── M1 / MOTOR 1 ───┐
                                        │
                                        ▼
                                 MATRIX Mini R4
                                        ▲
                                        │
Motor DC derecho ────── M2 / MOTOR 2 ───┘
```
Función
Los motores permiten:
Sacar la bandeja.
Insertar la bandeja.
Detener automáticamente la inserción cuando se activa el final de carrera.
En el funcionamiento configurado:
Pulsador 3: saca la bandeja durante el tiempo establecido por el firmware.
Pulsador 4: introduce la bandeja hasta detectar el final de carrera.
---
🖥️ Pantallas
ATOM ArchBot utiliza dos LCD externas y la OLED integrada del MATRIX.
LCD 16×2 — I2C2
```text
LCD 16×2
   │
   │ I2C2
   ▼
MATRIX Mini R4
```
Se utiliza como interfaz principal para mensajes de funcionamiento, conexión, instrucciones y estados del robot.
---
LCD 16×4 — I2C3
```text
LCD 16×4
   │
   │ I2C3
   ▼
MATRIX Mini R4
```
Se utiliza para mostrar información más detallada, por ejemplo:
```text
Tierra:     Baja
Velocidad:  ■■■□
Ubicación:  San Fernando
IA:         Conectado
```
Durante una detección también puede mostrar información del objeto, confianza e ID.
---
OLED integrada
La OLED forma parte del MATRIX Mini R4 y no requiere cableado externo.
Puede utilizarse para:
Inicio del sistema.
Logo de ATOM ArchBot / Los Harneritos.
Estado de sensores.
Estado de cámara.
Diagnóstico.
Errores.
Datos técnicos durante pruebas.
---
🔘 Pulsadores externos
Los cuatro pulsadores están instalados en el lado izquierdo del MATRIX Mini R4.
Pulsador	Puerto	Acción principal
Pulsador 1	D1-L	Comenzar harneado
Pulsador 2	D2-L	Pausar / continuar
Pulsador 3	D3-L	Sacar bandeja
Pulsador 4	D4-L	Insertar bandeja
Cableado eléctrico
Cada pulsador utiliza dos conexiones:
```text
Puerto digital ─── Pulsador ─── GND
```
Por lo tanto:
```text
D1-L ─── Pulsador 1 ─── GND
D2-L ─── Pulsador 2 ─── GND
D3-L ─── Pulsador 3 ─── GND
D4-L ─── Pulsador 4 ─── GND
```
El firmware utiliza la entrada correspondiente para detectar la pulsación.
---
🛑 Final de carrera
El final de carrera está conectado en el lado izquierdo del MATRIX mediante A1-L.
Cableado real
```text
Final de carrera

COM ───────── GND
NO  ───────── A1-L
```
La entrada trabaja con lógica activa en LOW:
```text
Sin pulsar  → HIGH
Pulsado     → LOW
```
Cuando la bandeja alcanza su posición final:
```text
Final de carrera activado
          ↓
A1-L pasa a LOW
          ↓
MATRIX detecta bandeja insertada
          ↓
Motores DC se detienen
```
Esto evita que los motores sigan empujando la bandeja después de alcanzar su límite mecánico.
---
🌈 Barras RGB WS2812B
ATOM ArchBot utiliza dos barras WS2812B de 8 LEDs controladas independientemente.
RGB izquierdo — D2-R
```text
WS2812B izquierdo

DIN ───────── D2-R
5V  ───────── 5V
GND ───────── GND
```
RGB derecho — D3-R
```text
WS2812B derecho

DIN ───────── D3-R
5V  ───────── 5V
GND ───────── GND
```
> En los WS2812B se debe conectar el cable de señal al pin **DIN** de la barra, no al pin DOUT.
Al utilizar dos líneas de datos independientes, el firmware puede controlar cada barra por separado.
Esto permite realizar:
Animaciones simétricas.
Indicadores de funcionamiento.
Avisos de error.
Confirmación de botones.
Iluminación de la cámara.
Señales visuales durante un hallazgo.
Efectos específicos del modo de prueba/exhibición.
---
🔊 Buzzer integrado
El buzzer está integrado en el MATRIX Mini R4, por lo que no necesita cableado externo.
Se utiliza para:
Sonido de encendido.
Confirmación de pulsadores.
Inicio del harneado.
Cambio de estado.
Movimiento de bandeja.
Aviso de hallazgo.
Alertas de error.
Confirmación de comunicación con ATOM.IA.
---
🎛️ Botones integrados del MATRIX
Los botones incorporados en el MATRIX Mini R4 se utilizan para funciones auxiliares, diagnóstico y modos de prueba.
Su función puede variar durante el desarrollo del firmware sin modificar el conexionado físico, ya que forman parte del propio controlador.
---
⚡ Alimentación del sistema
El sistema utiliza un pack de baterías de 2 series (2S) protegido mediante un BMS 2S.
En la configuración 2S:
```text
Voltaje nominal aproximado: 7,4 V
Voltaje máximo cargado:      8,4 V
```
En el montaje actual se utilizan 4 portabaterías de 2 celdas, para un total de 8 celdas 18650. Al organizar los puntos equivalentes de las cuatro ramas, el pack corresponde eléctricamente a una configuración 2S4P.
```text
Grupo 1: 4 celdas en paralelo
Grupo 2: 4 celdas en paralelo

Grupo 1 + Grupo 2 = pack 2S4P
Total: 8 celdas 18650
```
---
🛡️ BMS 2S
El BMS protege el pack frente a condiciones eléctricas anormales y supervisa los dos grupos en serie.
El pack 2S4P necesita tres puntos eléctricos principales hacia el BMS:
```text
B- / 0 V ───── negativo del Grupo 1
B1 / BM ────── unión entre Grupo 1 y Grupo 2
B+ / 8,4 V ─── positivo del Grupo 2
```
Representación simplificada:
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
> El nombre exacto del punto central puede aparecer impreso como `B1`, `BM`, `B-1` u otra variante dependiendo del modelo de BMS. Se debe respetar siempre la serigrafía de la placa instalada.
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
La placa de carga 2S se integra en el sistema de batería respetando la entrada/salida protegida definida por el BMS y la serigrafía de ambas placas.
---
🔌 Jack de alimentación
La salida del sistema de baterías llega al MATRIX Mini R4 mediante un conector DC 5.5 × 2.1 mm.
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
🗺️ Diagrama completo del conexionado
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
🚦 Estados principales
Las conexiones anteriores permiten implementar los principales estados de funcionamiento del robot.
Estado	Comportamiento
`INICIALIZANDO`	Comprobación de componentes y posición inicial
`LISTO`	Sistema preparado para comenzar
`HARNEANDO`	Harnero activo y sensores funcionando
`PAUSADO`	Movimiento detenido temporalmente
`HALLAZGO`	Movimiento detenido y alertas activadas
`ERROR`	Sistema detenido por una condición de seguridad
`FINALIZADO`	Proceso terminado
`EXHIBICION / PRUEBA`	Funciones de demostración y diagnóstico
---
🏺 ATOM ArchBot
Sistema robótico de apoyo al harneado arqueológico
El objetivo del sistema electrónico es integrar sensado, movimiento, visión artificial, control de bandeja, seguridad, información visual, alertas y comunicación con ATOM.IA en una única plataforma controlada mediante el MATRIX Mini R4.
