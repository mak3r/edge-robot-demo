#!/usr/bin/python3

import signal
import buttonshim
import os
import subprocess
import threading
import time
import sys

print("""
Gitops deployments for robot-demo gesture configuration
""")

SRC_DIR = "git-ops-demo"
CONFIG_DIR = "k8s-config"
FILE_NAME = "gesture-cm.yaml"

try:
    RUN_DIR = sys.argv[1]
    os.chdir(RUN_DIR)
except FileNotFoundError as fnfe:
    print("Working directory does not exist. Please pass a valid working directory to the script.")
    print(fnfe)
except IndexError as iee:
    print("The operational directory must exist and is the first parameter to this script")

CONFIG_FILE_PATH = RUN_DIR + "/" + SRC_DIR + "/" + CONFIG_DIR + "/" + FILE_NAME
SRC_PATH = RUN_DIR + "/" + SRC_DIR

##############
# Button A
##############
buttonA_was_held = False
cloneGitRepo =["git", "clone", "git@github.com:mak3r/edge-robot-demo.git", SRC_PATH]
removeGitRepo = ["rm", "-rf", SRC_PATH]

@buttonshim.on_press(buttonshim.BUTTON_A)
def button_A_press(button, pressed):
    global buttonA_was_held
    buttonshim.set_pixel(0x94, 0x00, 0xd3)
    buttonA_was_held = False

@buttonshim.on_release(buttonshim.BUTTON_A)
def button_A_release(button, pressed):
    global buttonA_was_held
    if not buttonA_was_held:
        # Checkout the repository into our working directory
        output = subprocess.check_output(cloneGitRepo, stderr=sys.stdout)
        print(output)


@buttonshim.on_hold(buttonshim.BUTTON_A, hold_time=2)
def button_A_hold(button):
    global buttonA_was_held
    buttonA_was_held = True
    output = subprocess.check_output(removeGitRepo, stderr=sys.stdout)
    print(output)

##############
# Button B
##############
buttonB_was_held = False
pullGitRepo = ["git", "-C", SRC_PATH, "pull"]
fetchGitRepo = ["git", "-C", SRC_PATH, "fetch"]


@buttonshim.on_press(buttonshim.BUTTON_B)
def button_B_press(button, pressed):
    global buttonB_was_held
    buttonshim.set_pixel(0x00, 0x00, 0xff)
    buttonB_was_held = False

@buttonshim.on_release(buttonshim.BUTTON_B)
def button_B_release(button, pressed):
    global buttonB_was_held
    if not buttonB_was_held:
        # Checkout the repository into our working directory
        output = subprocess.check_output(pullGitRepo, stderr=sys.stdout)
        print(output)

@buttonshim.on_hold(buttonshim.BUTTON_B, hold_time=2)
def button_B_hold(button):
    global buttonB_was_held
    buttonB_was_held = True
    output = subprocess.check_output(fetchGitRepo, stderr=sys.stdout)
    print(output)

##############
# Gestures
##############
configCountGesture = ["sed", "-i", "s/gesture=[a-z]*.py/gesture=count.py/", CONFIG_FILE_PATH ]
configPointgGesture = ["sed", "-i", "s/gesture=[a-z]*.py/gesture=point.py/", CONFIG_FILE_PATH ]
defaultGesture = ["sed", "-i", "s/gesture=[a-z]*.py/gesture=default.py/", CONFIG_FILE_PATH ]


##############
# Button C
##############
buttonC_was_held = False
curGesture = None

@buttonshim.on_press(buttonshim.BUTTON_C)
def button_C_press(button, pressed):
    global buttonC_was_held
    buttonshim.set_pixel(0x00, 0xff, 0x00)
    buttonC_was_held = False

@buttonshim.on_release(buttonshim.BUTTON_C)
def button_C_release(button, pressed):
    global buttonC_was_held
    global curGesture
    if not buttonC_was_held:
        print("curdir: " + os.getcwd())
        # Checkout the repository into our working directory
        output = subprocess.check_output(configCountGesture, stderr=sys.stdout)
        curGesture = "count gesture"
        print(output)

@buttonshim.on_hold(buttonshim.BUTTON_C, hold_time=2)
def button_C_hold(button):
    global buttonC_was_held
    global curGesture
    buttonC_was_held = True
    output = subprocess.check_output(defaultGesture, stderr=sys.stdout)
    curGesture = "default gesture"
    print(output)

##############
# Button D
##############
buttonD_was_held = False

@buttonshim.on_press(buttonshim.BUTTON_D)
def button_D_press(button, pressed):
    global buttonD_was_held
    buttonshim.set_pixel(0xff, 0xff, 0x00)
    buttonD_was_held = False

@buttonshim.on_release(buttonshim.BUTTON_D)
def button_D_release(button, pressed):
    global buttonD_was_held
    global curGesture
    if not buttonD_was_held:
        # Checkout the repository into our working directory
        output = subprocess.check_output(configPointgGesture, stderr=sys.stdout)
        curGesture = "point gesture"
        print(output)

@buttonshim.on_hold(buttonshim.BUTTON_D, hold_time=2)
def button_D_hold(button):
    global buttonD_was_held
    global curGesture
    buttonD_was_held = True
    output = subprocess.check_output(defaultGesture, stderr=sys.stdout)
    curGesture = "default gesture"
    print(output)

##############
# Button E
##############

buttonE_was_held = False

def gitPush():
    global curGesture
    gitAdd = ["git", "-C", SRC_PATH, "add", "--all"]
    gitCommit = ["git", "-C", SRC_PATH, "commit", "-m", curGesture]
    pushGitRepo = ["git", "-C", SRC_PATH, "push"]
    try:
        output = subprocess.check_output(gitAdd, stderr=sys.stdout)
        print(output)
        output = subprocess.check_output(gitCommit, stderr=sys.stdout)
        print(output)
        output = subprocess.check_output(pushGitRepo, stderr=sys.stdout)
        print(output)
    except Exception as e:
        print(e)

@buttonshim.on_press(buttonshim.BUTTON_E)
def button_E_press(button, pressed):
    global buttonE_was_held
    global E_count
    buttonshim.set_pixel(0xff, 0x00, 0x00)
    buttonE_was_held = False

@buttonshim.on_release(buttonshim.BUTTON_E)
def button_E_release(button, pressed):
    global buttonE_was_held
    if not buttonE_was_held:
        # git push code 
        gitPush()

@buttonshim.on_hold(buttonshim.BUTTON_E, hold_time=2)
def button_E_hold(button):
    global buttonE_was_held
    buttonE_was_held = True
    # git push code 
    gitPush()


signal.pause()