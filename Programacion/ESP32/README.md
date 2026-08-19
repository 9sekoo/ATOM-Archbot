# ESP32 — ATOM.IA

Esta carpeta contiene la programación correspondiente a la ESP32 utilizada en el proyecto ATOM-ArchBot.

La ESP32 cumple principalmente la función de alojar ATOM.IA, conectarse a Internet y servir como puente de comunicación entre el robot y la interfaz web.

## Funciones principales

- Alojar la página web de ATOM.IA.
- Conectarse automáticamente a una red Wi-Fi.
- Comunicarse con Groq para generar respuestas de inteligencia artificial.
- Mantener una red local de respaldo llamada `ATOM-ARCHBOT`.
- Recibir información enviada por el MATRIX Mini R4.
- Recibir eventos de posibles hallazgos detectados por la M-Vision.
- Mostrar el estado de ATOM-ArchBot en la interfaz web.
- Mantener funciones locales incluso si no hay conexión a Internet.
- Mostrar el estado de conexión con Internet, Groq y ATOM-ArchBot.
- Permitir el acceso mediante `atom-ia.local` o mediante una dirección IP local.

## Comunicación del sistema

La comunicación general de ATOM-ArchBot funciona de la siguiente manera:

```text
M-Vision Cam
     ↓
MATRIX Mini R4
     ↓ Wi-Fi
ESP32
     ↓
ATOM.IA
