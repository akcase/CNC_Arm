import os
import signal
from gpiozero import DigitalInputDevice
from gpiozero import DigitalOutputDevice
import time
import threading
import linuxcnc

start_program_pin = DigitalInputDevice(19, pull_up=False) # Triggered by GUI Pi, tells to start program
end_program_pin = DigitalOutputDevice(20)  # Triggered by CNC Pi, says program is ended
homing_pin = DigitalInputDevice(6, pull_up=False)
cnc_pi_on_pin = DigitalOutputDevice(5) # Pin 29

os.system("sudo -u cnc linuxcnc Desktop/PortaGuide/PortaGuide.ini &")
os.system('\x03')

time.sleep(2)

cnc_pi_on_pin.on()

c = linuxcnc.command()
s = linuxcnc.stat()

c.state(linuxcnc.STATE_ESTOP_RESET)
c.state(linuxcnc.STATE_ON)

c.home(0)
c.home(1)

estop_pressed = False

# Interrupt Stuff

#def handle_estop():
#	global estop_pressed
#	c.unhome(0)
#	c.wait_complete
#	c.unhome(1)
#	c.wait_complete()
#	while (homing_pin.value == 1):
#		pass
#	c.state(linuxcnc.STATE_ESTOP_RESET)
#	c.wait_complete()
#	estop_pressed = False
#	c.home(0)
#	c.home(1)
#	c.wait_complete()
#
#def poll():
#	global estop_pressed
#	s.poll()
#	if s.estop:
#		estop_pressed = True
#		c.abort()
#		handle_estop()
#
#def check_for_estop():
#	while (1):
#		poll()
#		time.sleep(0.5)
#
#timer_interrupt = threading.Thread(target=check_for_estop)
#timer_interrupt.daemon = True
#timer_interrupt.start()
 
def ok_to_run():
	s.poll()
	return not s.estop and s.enabled and s.homed

def run_mdi_cmds():
	global estop_pressed

	with open("../../home/cnc/Desktop/PortaGuide/output_file.ngc", 'r') as file:
		lines = file.readlines()

	if estop_pressed:
		return

	for line in lines:
		if estop_pressed:
			return
		line = line.strip()
		c.mdi(line)
		if "M02" in line:
			return

def run_program():
	# Wait for start pin to trigger from GUI Pi
	while (start_program_pin.value == 0):
		pass

	if estop_pressed:
		return

	print("Starting LinuxCNC Program!")
	if ok_to_run:
		c.mode(linuxcnc.MODE_MDI)
		c.wait_complete()
		c.state(linuxcnc.STATE_ON)
		run_mdi_cmds()

	if estop_pressed:
		return

	s.poll()

	while (s.state == linuxcnc.RCS_EXEC):
		s.poll()
		time.sleep(1)


	if estop_pressed:
		return

	if ok_to_run:
		c.mode(linuxcnc.MODE_MDI)
		c.wait_complete()
		c.mdi("G1 F1000 X0 Y0")

	end_program_pin.off()


	if estop_pressed:
		return

	s.poll()

	while (s.state != linuxcnc.RCS_DONE):
		s.poll()
		print("Waiting 2")
		time.sleep(1)

	end_program_pin.on() # Signal that program is finished

	print("All done :)")
	end_program_pin.off()

while (1):
	s.poll()
	if not estop_pressed:
		run_program()
	time.sleep(1)
