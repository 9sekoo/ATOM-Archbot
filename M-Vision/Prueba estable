import sensor
import time
import pyb
import fill_light


# ─────────────────────────────────────────────
# ATOM ARCHBOT - SISTEMA DE VISION
# M-Vision / OpenMV
# ─────────────────────────────────────────────
#
# Funciones principales:
#
# - Detecta piezas mediante color LAB.
# - Calcula un índice de confianza.
# - Suaviza la confianza entre varios frames.
# - Solo confirma detecciones estables >= 70%.
# - Sigue la posición del objeto para evitar saltos.
# - Utiliza iluminación blanca normalmente.
# - Cada 2 segundos muestra un breve indicador:
#
#       ROJO  = confianza menor a 70%
#       VERDE = confianza igual o mayor a 70%
#
# ─────────────────────────────────────────────


# ─────────────────────────────────────────────
# Configuración de cámara
# ─────────────────────────────────────────────

CAMARA_GIRADA = True

RESOLUCION = sensor.QVGA       # 320 x 240

# Ventana real utilizada por la cámara.
# Menor altura = menos procesamiento y más FPS.

VENTANA_CAMARA = (
    0,
    35,
    320,
    170
)

BRILLO_LUZ_BLANCA = 35


# ─────────────────────────────────────────────
# Estabilidad de detección
# ─────────────────────────────────────────────

CONFIANZA_MINIMA = 70

# Cantidad de porcentajes utilizados para
# calcular el promedio estable.

VENTANA_CONFIANZA = 6

# Frames sobre 70% antes de informar al MATRIX.

FRAMES_CONFIRMAR = 8

# Frames que se puede perder temporalmente
# el objeto sin reiniciar el seguimiento.

FRAMES_PERDIDOS_MAX = 5

# Tiempo sin objeto antes de permitir
# detectar una nueva pieza.

FRAMES_REARMAR = 20

# Movimiento máximo del objeto entre frames.

MAX_SALTO = 45


# ─────────────────────────────────────────────
# Indicador RGB
# ─────────────────────────────────────────────

# El RGB permanece blanco normalmente.

# Cada 2 segundos muestra el estado durante
# 150 ms y luego vuelve a blanco.

INTERVALO_ESTADO_MS = 2000
DURACION_ESTADO_MS = 150


# ─────────────────────────────────────────────
# Interfaz visual
# ─────────────────────────────────────────────

MOSTRAR_CUADRO = True
MOSTRAR_ETIQUETA = True


# ─────────────────────────────────────────────
# Objetos
# ─────────────────────────────────────────────

OBJETOS = [

    {
        "nombre": "PIEZA",

        # Rango LAB del color buscado

        "threshold": (
            0, 81,
            -38, 123,
            16, 38
        ),

        # Color visual del cuadro

        "color_cuadro": (0, 255, 0),

        # Filtros de tamaño

        "min_pixels": 700,
        "min_area": 700,

        "min_width": 22,
        "max_width": 180,

        "min_height": 22,
        "max_height": 160,

        "min_density": 0.16
    },


    # ─────────────────────────────────────────
    # EJEMPLO DE OTRO OBJETO
    # ─────────────────────────────────────────
    #
    # Para agregar otro objeto:
    #
    # 1. Añadir una coma al objeto anterior.
    # 2. Quitar los comentarios.
    # 3. Cambiar nombre, threshold y color.
    #
    #
    # {
    #     "nombre": "CERAMICA",
    #
    #     "threshold": (
    #         20, 70,
    #         20, 80,
    #         10, 60
    #     ),
    #
    #     "color_cuadro": (255, 0, 0),
    #
    #     "min_pixels": 600,
    #     "min_area": 600,
    #
    #     "min_width": 20,
    #     "max_width": 180,
    #
    #     "min_height": 20,
    #     "max_height": 160,
    #
    #     "min_density": 0.15
    # }

]


# ─────────────────────────────────────────────
# Comunicación USB
# ─────────────────────────────────────────────

usb = pyb.USB_VCP()
usb.setinterrupt(-1)


# ─────────────────────────────────────────────
# LEDs RGB
# ─────────────────────────────────────────────

led_rojo = pyb.LED(1)
led_verde = pyb.LED(2)
led_azul = pyb.LED(3)


def rgb_apagado():

    led_rojo.off()
    led_verde.off()
    led_azul.off()


def rgb_blanco():

    led_rojo.on()
    led_verde.on()
    led_azul.on()


def rgb_rojo():

    led_rojo.on()
    led_verde.off()
    led_azul.off()


def rgb_verde():

    led_rojo.off()
    led_verde.on()
    led_azul.off()


# ─────────────────────────────────────────────
# Iluminación blanca
# ─────────────────────────────────────────────

fill_light.brightness(
    BRILLO_LUZ_BLANCA
)

fill_light.on()


# ─────────────────────────────────────────────
# Inicialización de cámara
# ─────────────────────────────────────────────

sensor.reset()

if CAMARA_GIRADA:

    sensor.set_vflip(True)
    sensor.set_hmirror(True)


sensor.set_pixformat(
    sensor.RGB565
)

sensor.set_framesize(
    RESOLUCION
)


# Recortar físicamente el frame.
# Esto reduce la cantidad de píxeles procesados
# y normalmente permite obtener más FPS.

sensor.set_windowing(
    VENTANA_CAMARA
)


sensor.skip_frames(
    time=2000
)


# ─────────────────────────────────────────────
# Bloquear ajustes automáticos
# ─────────────────────────────────────────────
#
# Evita que exposición, ganancia y balance
# cambien continuamente y alteren los colores.
# ─────────────────────────────────────────────

gain = sensor.get_gain_db()
rgb_gain = sensor.get_rgb_gain_db()
exposure = sensor.get_exposure_us()


sensor.set_auto_gain(
    False,
    gain_db=gain
)


sensor.set_auto_whitebal(
    False,
    rgb_gain_db=rgb_gain
)


sensor.set_auto_exposure(
    False,
    exposure_us=exposure
)


# ─────────────────────────────────────────────
# Estado del sistema
# ─────────────────────────────────────────────

frames_perdidos = 0
frames_sin_objeto = 0
frames_confianza_valida = 0

objeto_confirmado = False

ultimo_cx = None
ultimo_cy = None

ultimo_blob = None
ultimo_config = None

confianza_suavizada = 0

historial_confianza = []


# Estado RGB

ultimo_flash = pyb.millis()

flash_activo = False
inicio_flash = 0


# Diagnóstico

ultimo_debug = pyb.millis()


# Comenzar iluminando en blanco

rgb_blanco()


# ─────────────────────────────────────────────
# Distancia entre posiciones
# ─────────────────────────────────────────────

def distancia(x1, y1, x2, y2):

    dx = x2 - x1
    dy = y2 - y1

    return (
        dx * dx +
        dy * dy
    ) ** 0.5


# ─────────────────────────────────────────────
# Validar objeto
# ─────────────────────────────────────────────

def blob_valido(blob, config):

    ancho = blob.w()
    alto = blob.h()

    area = ancho * alto

    if area <= 0:
        return False


    pixels = blob.pixels()

    densidad = pixels / area


    if pixels < config["min_pixels"]:
        return False


    if area < config["min_area"]:
        return False


    if ancho < config["min_width"]:
        return False


    if ancho > config["max_width"]:
        return False


    if alto < config["min_height"]:
        return False


    if alto > config["max_height"]:
        return False


    if densidad < config["min_density"]:
        return False


    return True


# ─────────────────────────────────────────────
# Calcular confianza
# ─────────────────────────────────────────────
#
# Este porcentaje no corresponde a una red
# neuronal.
#
# Es un índice de coincidencia utilizado para
# evaluar la calidad de la detección.
#
# Considera:
#
# - densidad del color
# - píxeles detectados
# - tamaño de la región
#
# ─────────────────────────────────────────────

def calcular_confianza(blob, config):

    ancho = blob.w()
    alto = blob.h()

    area = ancho * alto

    if area <= 0:
        return 0


    pixels = blob.pixels()

    densidad = pixels / area


    # Densidad

    score_densidad = (
        densidad * 100
    )

    if score_densidad > 100:
        score_densidad = 100


    # Píxeles

    referencia_pixels = (
        config["min_pixels"] * 3
    )

    score_pixels = (
        pixels /
        referencia_pixels
    ) * 100

    if score_pixels > 100:
        score_pixels = 100


    # Área

    referencia_area = (
        config["min_area"] * 3
    )

    score_area = (
        area /
        referencia_area
    ) * 100

    if score_area > 100:
        score_area = 100


    # Resultado

    confianza = (

        score_densidad * 0.60 +

        score_pixels * 0.25 +

        score_area * 0.15
    )


    if confianza > 99:
        confianza = 99


    if confianza < 1:
        confianza = 1


    return int(confianza)


# ─────────────────────────────────────────────
# Suavizar confianza
# ─────────────────────────────────────────────

def suavizar_confianza(valor):

    historial_confianza.append(
        valor
    )


    if (
        len(historial_confianza) >
        VENTANA_CONFIANZA
    ):

        historial_confianza.pop(0)


    suma = 0


    for valor_guardado in historial_confianza:

        suma += valor_guardado


    promedio = (
        suma /
        len(historial_confianza)
    )


    return int(promedio)


# ─────────────────────────────────────────────
# Buscar objetos
# ─────────────────────────────────────────────

def detectar_objetos(img):

    detecciones = []


    for config in OBJETOS:

        blobs = img.find_blobs(

            [config["threshold"]],

            pixels_threshold=300,
            area_threshold=300,

            merge=False,
            margin=0
        )


        mejor_blob = None


        # ───────── Seguimiento ─────────

        if ultimo_cx is not None:

            mejor_distancia = 99999


            for blob in blobs:

                if not blob_valido(
                    blob,
                    config
                ):

                    continue


                d = distancia(

                    ultimo_cx,
                    ultimo_cy,

                    blob.cx(),
                    blob.cy()
                )


                if d < mejor_distancia:

                    mejor_distancia = d
                    mejor_blob = blob


            if mejor_distancia > MAX_SALTO:

                mejor_blob = None


        # ───────── Nueva detección ─────────

        if mejor_blob is None:

            mejor_pixels = 0


            for blob in blobs:

                if not blob_valido(
                    blob,
                    config
                ):

                    continue


                if blob.pixels() > mejor_pixels:

                    mejor_pixels = blob.pixels()
                    mejor_blob = blob


        # ───────── Resultado ─────────

        if mejor_blob is not None:

            confianza = calcular_confianza(

                mejor_blob,
                config
            )


            detecciones.append(

                (
                    config,
                    mejor_blob,
                    confianza
                )

            )


    return detecciones


# ─────────────────────────────────────────────
# Dibujar interfaz
# ─────────────────────────────────────────────

def dibujar_deteccion(
    img,
    config,
    blob,
    confianza
):

    color = config[
        "color_cuadro"
    ]


    # Cuadro

    if MOSTRAR_CUADRO:

        img.draw_rectangle(

            blob.rect(),

            color=color,
            thickness=2
        )


    # Etiqueta

    if MOSTRAR_ETIQUETA:

        texto = (

            config["nombre"] +

            " " +

            str(confianza) +

            "%"
        )


        ancho_texto = (
            len(texto) * 8 + 6
        )

        alto_texto = 14


        x = blob.x()

        y = (
            blob.y() -
            alto_texto
        )


        if y < 0:

            y = blob.y()


        # Fondo

        img.draw_rectangle(

            x,
            y,

            ancho_texto,
            alto_texto,

            color=color,
            fill=True
        )


        # Texto

        img.draw_string(

            x + 3,
            y + 2,

            texto,

            color=(0, 0, 0),
            scale=1
        )


# ─────────────────────────────────────────────
# Indicador RGB periódico
# ─────────────────────────────────────────────

def actualizar_rgb(confianza):

    global ultimo_flash
    global flash_activo
    global inicio_flash


    ahora = pyb.millis()


    # Iniciar indicador

    if (

        not flash_activo

        and

        pyb.elapsed_millis(
            ultimo_flash
        ) >= INTERVALO_ESTADO_MS

    ):

        flash_activo = True

        inicio_flash = ahora


        if confianza >= CONFIANZA_MINIMA:

            rgb_verde()

        else:

            rgb_rojo()


    # Volver a blanco

    if flash_activo:

        if (

            pyb.elapsed_millis(
                inicio_flash
            )

            >=

            DURACION_ESTADO_MS

        ):

            flash_activo = False

            ultimo_flash = ahora

            rgb_blanco()


# ─────────────────────────────────────────────
# Enviar evento al MATRIX
# ─────────────────────────────────────────────

def enviar_evento(
    nombre,
    confianza
):

    mensaje = (

        "DETECTADO:" +

        nombre +

        ":" +

        str(confianza) +

        "%\n"
    )


    print(mensaje)


    try:

        usb.write(
            mensaje
        )

    except:

        pass


# ─────────────────────────────────────────────
# Reiniciar detección
# ─────────────────────────────────────────────

def reiniciar_deteccion():

    global objeto_confirmado

    global ultimo_cx
    global ultimo_cy

    global ultimo_blob
    global ultimo_config

    global confianza_suavizada

    global frames_perdidos
    global frames_sin_objeto
    global frames_confianza_valida

    global historial_confianza


    objeto_confirmado = False


    ultimo_cx = None
    ultimo_cy = None


    ultimo_blob = None
    ultimo_config = None


    confianza_suavizada = 0


    frames_perdidos = 0
    frames_sin_objeto = 0

    frames_confianza_valida = 0


    historial_confianza = []


# ─────────────────────────────────────────────
# Programa principal
# ─────────────────────────────────────────────

clock = time.clock()


while True:

    clock.tick()


    img = sensor.snapshot()


    detecciones = detectar_objetos(
        img
    )


    # ─────────────────────────────────────────
    # Objeto encontrado
    # ─────────────────────────────────────────

    if len(detecciones) > 0:


        config, blob, confianza_actual = (

            detecciones[0]
        )


        # Guardar posición

        ultimo_cx = blob.cx()
        ultimo_cy = blob.cy()

        ultimo_blob = blob
        ultimo_config = config


        # Reiniciar pérdidas

        frames_perdidos = 0
        frames_sin_objeto = 0


        # Suavizar porcentaje

        confianza_suavizada = suavizar_confianza(

            confianza_actual
        )


        # Mostrar detección

        dibujar_deteccion(

            img,
            config,
            blob,

            confianza_suavizada
        )


        # ─────────────────────────────────────
        # Evaluar confianza
        # ─────────────────────────────────────

        if confianza_suavizada >= CONFIANZA_MINIMA:


            frames_confianza_valida += 1


        else:


            if frames_confianza_valida > 0:

                frames_confianza_valida -= 1


        # ─────────────────────────────────────
        # Confirmar detección
        # ─────────────────────────────────────

        if (

            frames_confianza_valida >=

            FRAMES_CONFIRMAR

        ):


            if not objeto_confirmado:


                objeto_confirmado = True


                enviar_evento(

                    config["nombre"],

                    confianza_suavizada
                )


    # ─────────────────────────────────────────
    # Sin objeto
    # ─────────────────────────────────────────

    else:


        frames_perdidos += 1


        # Mantener seguimiento brevemente

        if (

            frames_perdidos >

            FRAMES_PERDIDOS_MAX

        ):


            frames_sin_objeto += 1


            if frames_confianza_valida > 0:

                frames_confianza_valida -= 1


            # Rearmar sistema

            if (

                frames_sin_objeto >=

                FRAMES_REARMAR

            ):

                reiniciar_deteccion()


    # ─────────────────────────────────────────
    # RGB de estado
    # ─────────────────────────────────────────

    actualizar_rgb(
        confianza_suavizada
    )


    # ─────────────────────────────────────────
    # Diagnóstico cada 1 segundo
    # ─────────────────────────────────────────

    if (

        pyb.elapsed_millis(
            ultimo_debug
        ) >= 1000

    ):


        ultimo_debug = pyb.millis()


        print(

            "FPS:",

            int(clock.fps()),

            "| Confianza:",

            confianza_suavizada,

            "%",

            "| Estabilidad:",

            frames_confianza_valida,

            "/",

            FRAMES_CONFIRMAR

        )  
