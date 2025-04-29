# PNP Arm A Telecontrol

# PuppetArmControl

This project enables the motion control of a 4 DOF robotic arm using a puppet equipped with potentiometers. The puppet’s joint angles are read in real-time and mimicked by the robotic arm using servo motors. The system also features programmable modes such as Save/Edit, Delete, Cycle, and Mirror, allowing pose capture, playback, and real-time mirroring for enhanced functionality.

<p align="center">
  <a href="https://youtu.be/VsuDK407MbY">
    <img src="Demo.gif" alt="Project Demo" width="848" height="382">
  </a>
</p>

---

## Key Features
- **Real-Time Mirroring**: The robotic arm replicates the puppet’s pose in real-time using potentiometer readings.
- **State Programming**: Save, edit, delete, or cycle through up to 10 programmable states.
- **Versatile Functionality**: Includes modes for discrete state execution and reference alignment.
- **User-Friendly Interface**: Controlled using a 4x4 keypad for easy interaction.

---

## Components Used
- **Arduino Mega**: The microcontroller, chosen for its numerous I/O pins and affordability.
- **10 kΩ Potentiometers**: Measure the puppet's joint angles to control the robotic arm.
- **SG-90 Servo Motors**: Provide precise actuation for the robotic arm's joints.
- **4x4 Keypad**: Facilitates mode selection and state manipulation.
- **12V SMPS & Buck Converter**: Provide power and voltage regulation for all components.

---

## How It Works

### Measuring Puppet Pose
- Potentiometers attached to the puppet’s joints output analog signals (0–1023) to the Arduino Mega.
- These values are mapped to the physical angular constraints of the robotic arm.
- Noise is minimized by filtering outliers and averaging 10 continuous readings.

### Saving a Pose
- Joint angles are saved in the Arduino Mega’s EEPROM memory.
- Up to 10 poses can be stored, edited, or deleted.
- Poses are tagged with a state ID (0–9) for easy identification.

---

## Modes of Operation

### 1. **Save/Edit**
Capture and store the puppet’s pose or edit an existing state.
#### Steps:
1. Press the **Save/Edit** button on the keypad.
2. Select the desired state ID (0–9).
3. If editing, confirm overwriting the existing state.
4. Position the puppet in the desired pose.
5. Press **Save/Edit** again to save the pose.

### 2. **Delete**
Delete a saved state from the EEPROM.
#### Steps:
1. Press the **Delete** button on the keypad.
2. Select the state ID to delete.
3. Confirm the deletion by re-pressing the state ID.

### 3. **Cycle**
Cycle through all saved states sequentially in a loop.
#### Steps:
1. Press the **Cycle** button.
2. The robot checks for gaps in the state sequence.
3. If no gaps are found, the arm cycles through the states repeatedly.
4. Cancel the cycle by pressing the **C

