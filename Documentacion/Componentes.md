# Componentes de ATOM ArchBot

Este documento reúne los principales componentes electrónicos, mecánicos y de control utilizados en ATOM ArchBot.

---

## Control principal

### MATRIX Mini R4
**Cantidad:** 1

Es el controlador principal de ATOM.

Se encargará de:

- Coordinar los estados del sistema.
- Leer sensores.
- Controlar servomotores y motores DC.
- Recibir información de la M-Vision.
- Controlar LEDs RGB.
- Manejar las pantallas.
- Leer los pulsadores.
- Controlar el buzzer.
- Ejecutar funciones de seguridad y autodiagnóstico.

Además se aprovecharán componentes que ya vienen integrados en el MATRIX, como:

- Pantalla OLED.
- Buzzer.
- Botones integrados.
- LEDs RGB integrados.

---

# Sistema de visión

### M-Vision Cam
**Cantidad:** 1

Es la cámara encargada de la visión artificial.

Sus funciones principales serán:

- Observar la zona de harneado.
- Detectar posibles objetos arqueológicos.
- Analizar características visuales.
- Calcular un nivel de confianza.
- Confirmar detecciones durante varios frames.
- Obtener la posición aproximada del objeto.
- Enviar información al MATRIX Mini R4 mediante UART.

La cámara tendrá una función especialmente importante en la seguridad del sistema, ya que una detección válida provocará la detención inmediata del harneado.

---

# Servomotores

### Servomotores MATRIX de engranajes metálicos
**Cantidad:** 2

Serán utilizados para generar el movimiento de oscilación del sistema de harneado.

Su velocidad de movimiento dependerá de las mediciones realizadas por los sensores láser.

De forma general:

- Mayor cantidad de tierra → oscilación más rápida.
- Menor cantidad de tierra → oscilación más lenta.
- Posible hallazgo → ambos servos regresan a 0°.

Los engranajes metálicos permiten utilizar los servos en un mecanismo sometido a mayor esfuerzo.

---

# Motores DC

### Motores DC MATRIX
**Cantidad:** 2

Los motores DC estarán encargados del movimiento de la bandeja de tierra.

Funciones:

- Extraer la bandeja.
- Insertar la bandeja.
- Detener el movimiento cuando corresponda.

El movimiento será controlado mediante los pulsadores 3 y 4.

---

# Sensores láser

### Sensores láser MATRIX
**Cantidad:** 2

Serán utilizados para medir la distancia entre los sensores y el material presente en el harnero.

Las lecturas permitirán estimar aproximadamente la cantidad de tierra disponible.

Los datos de ambos sensores serán procesados mediante un algoritmo de filtrado antes de modificar el movimiento de los servomotores.

Esto permitirá reducir errores causados por:

- Vibraciones.
- Tierra en movimiento.
- Lecturas aisladas.
- Diferencias momentáneas entre sensores.
- Objetos presentes en el harnero.

---

# Final de carrera

### Final de carrera MATRIX
**Cantidad:** 1

Será utilizado principalmente para controlar la inserción de la bandeja.

Cuando la bandeja llegue a su posición final:

1. Se activa el final de carrera.
2. El MATRIX detecta la señal.
3. Se detienen automáticamente los motores DC.

Esto evita que el mecanismo continúe empujando la bandeja indefinidamente.

---

# Pulsadores de control

### Pulsadores
**Cantidad:** 4

Cada pulsador tendrá una función específica.

### Pulsador 1 — Inicio

Inicia el proceso de harneado cuando el sistema se encuentra listo.

### Pulsador 2 — Pausa / Continuar

Permite detener temporalmente el proceso.

Una nueva pulsación permite continuar.

### Pulsador 3 — Sacar bandeja

Activa los motores DC para retirar la bandeja.

### Pulsador 4 — Insertar bandeja

Activa los motores DC para introducir la bandeja.

El final de carrera detendrá automáticamente el movimiento cuando llegue a la posición correcta.

---

# Interruptor principal

### Pulsador con bloqueo
**Cantidad:** 1

Será utilizado como control físico de encendido y apagado del sistema.

Al ser un pulsador con bloqueo, mantiene su posición hasta volver a ser presionado.

Su función será permitir un encendido y apagado claramente identificable para el usuario.

---

# Iluminación e indicadores

### Tiras LED RGB
**Cantidad:** 4

Las tiras RGB servirán como indicadores visuales del estado de ATOM.

Algunos estados previstos son:

- Encendido.
- Inicialización.
- Sistema listo.
- Harneando.
- Pausado.
- Hallazgo.
- Error.
- Fin de harneado.

También realizarán una animación de encendido para indicar visualmente que ATOM recibió alimentación correctamente.

---

# Pantallas

## LCD I2C 16x2
**Cantidad:** 1

Funcionará principalmente como interfaz sencilla para el usuario.

Ejemplos:

```text
COMENZAR
HARNEADO?
