# Pruebas de programación — ATOM-ArchBot

Esta carpeta contiene programas experimentales y pruebas independientes de los componentes de **ATOM-ArchBot**.

El código guardado aquí puede estar incompleto o en desarrollo. Cuando una prueba funcione de manera estable, el código limpio se copiará a la carpeta de programación definitiva del robot.

## Objetivos

* Probar cada componente por separado.
* Detectar errores antes de integrar el programa completo.
* Comprobar la estabilidad de las comunicaciones.
* Registrar resultados, problemas y soluciones.
* Conservar versiones que hayan funcionado correctamente.

## Organización

```text
Pruebas_programacion/
├── Matrix_Mini_R4/
├── M_Vision/
├── ESP32_ATOM_IA/
├── Sensores/
├── Actuadores/
└── Integracion/
```

### Matrix Mini R4

Pruebas relacionadas con:

* Movimiento y control del robot.
* Motores y servomotores.
* Finales de carrera.
* Sensores.
* Botones y estados del sistema.
* Comunicación con el ESP32.

### M-Vision

Pruebas relacionadas con:

* Detección de objetos.
* Reconocimiento de la punta de proyectil.
* Reconocimiento del sello.
* Nivel de confianza visual.
* Comunicación UART con MATRIX.
* Iluminación y distancia de detección.

> Una coincidencia visual no confirma que un objeto sea arqueológico. El resultado siempre debe ser revisado por una persona.

### ESP32 y ATOM.ia

Pruebas relacionadas con:

* Punto de acceso Wi-Fi.
* Servidor web local.
* Comunicación con ATOM.ia.
* Heartbeat del MATRIX.
* Estados ONLINE y OFFLINE.
* Reconexión automática.
* Envío y recepción de eventos.

### Sensores y actuadores

Pruebas individuales de:

* Sensores de distancia.
* Sensor ultrasónico.
* Sensores láser.
* Motores.
* Servomotores.
* Luces, botones y finales de carrera.

### Integración

Aquí se guardan las pruebas que utilizan dos o más componentes al mismo tiempo, por ejemplo:

* MATRIX + ESP32.
* MATRIX + M-Vision.
* MATRIX + sensores y motores.
* Sistema completo del robot.

## Estado de las pruebas

| Estado         | Significado                |
| -------------- | -------------------------- |
| 🔴 No funciona | La prueba presenta errores |
| 🟡 En pruebas  | Funciona parcialmente      |
| 🟢 Estable     | Funciona correctamente     |
| 🔵 Pendiente   | Todavía no se ha probado   |

## Registro inicial

| Prueba                | Componentes        | Estado        | Resultado                                        |
| --------------------- | ------------------ | ------------- | ------------------------------------------------ |
| Heartbeat y eventos   | MATRIX + ESP32     | 🟢 Estable    | ATOM.ia recibe el estado del robot               |
| Reconexión automática | ESP32 + página web | 🟡 En pruebas | Se debe comprobar durante periodos largos        |
| Detección visual      | M-Vision           | 🟡 En pruebas | Falta probar distintas distancias e iluminación  |
| Movimiento y harneado | MATRIX + motores   | 🟡 En pruebas | Se debe validar con la alimentación definitiva   |
| Sistema completo      | Todos              | 🔵 Pendiente  | Se realizará después de las pruebas individuales |

## Cómo guardar una prueba

Cada prueba debe incluir:

1. El archivo de programación.
2. La fecha de la prueba.
3. Los componentes utilizados.
4. El objetivo.
5. Las conexiones realizadas.
6. El resultado obtenido.
7. Los errores encontrados.
8. La solución aplicada.

Ejemplo de nombre:

```text
2026-08-19_Matrix_Heartbeat
```

## Importante

* No modificar el código definitivo directamente desde esta carpeta.
* No subir contraseñas de Wi-Fi ni claves de API a GitHub.
* Reemplazar la información privada por ejemplos como `TU_WIFI`, `TU_PASSWORD` o `TU_API_KEY`.
* Una prueba se considerará estable únicamente después de repetirla varias veces sin errores.
