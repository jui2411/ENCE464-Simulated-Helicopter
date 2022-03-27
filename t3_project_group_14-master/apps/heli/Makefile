
# Specify tiva directory
TIVA_DIR = ../..

# Define our source and ammend files within the application directory
SRC = main.c flight_controller.c display_handler.c heading_handler.c led_controller.c ui_input_handler.c pilot.c altitude_handler.c rotors.c


#SRC += $(wildcard ./*.c)
SRC += $(UTILS)/math_ext/math_ext.c
SRC += $(UTILS)/pid/pid.c

include $(TIVA_DIR)/tiva.mk
