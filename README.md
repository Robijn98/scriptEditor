<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/Robijn98/scriptEditor/tree/master/dragAndDrop">
    <img src="dragAndDrop/bes.png" alt="Logo" width="300">
  </a>
  <h3 align="center">Bes Editor</h3>
  
</div>

<!-- ABOUT THE PROJECT -->
## About The Project

The Bes Editor is a Maya Autodesk plugin designed to improve your Maya workflow. 
The Bes Editor will allow for you to easily save and edit scripts and create templates for your rigs or other needs straight in the editor. 
It also makes sure everything you leave in the Editor is saved when you exit Maya, has splitscreen and syntax highlights your output. 
Easily rename your files or tabs and dock the plugin easily into your Maya.



### Built With
<div align="center">
  
[![C++](https://img.shields.io/badge/C++-00599C.svg?&logo=cplusplus&logoColor=white)](#)
[![CMake](https://img.shields.io/badge/build-CMake-blue?logo=cmake&logoColor=white)](#)
[![Linux](https://img.shields.io/badge/platform-Linux-green?logo=linux&logoColor=white)](#)
![Maya](https://img.shields.io/badge/Maya-00A3E0?&logo=autodesk&logoColor=white)
![Maya API](https://img.shields.io/badge/Maya--API-1D5C88?&logo=code&logoColor=white)
![cmds](https://img.shields.io/badge/Maya--cmds-232F3E?&logo=python&logoColor=white)


</div>

<!-- GETTING STARTED -->
## Getting Started
### Prerequisites

To run you will need the following 
- maya 2023
(will probably run in other version but hasn't been tested)

### Installation
Clone the repo
   ```sh
   git clone git@github.com:Robijn98/scriptEditor.git
   ```
<!-- USAGE EXAMPLES -->
## Usage

To use the program you can simply clone the repo or download the dragdrop folder. 
Make sure that when you are using the drag and drop both the editor and the 
install script are located in the same folder than drop the dragdrop in a maya scene,
this will put the editor into the right folder and create all needed folder. If the dragdrop doesn't work you can 
manually install by placing the plugin file in your maya plugin folder probably 
located at:

windows: drive:\\Program Files\\Autodesk\\Maya<version>\\bin\\plug-ins

mac: /Applications/Autodesk/maya<version>/Maya.app/Contents/MacOS/plug-ins

linux: /usr/autodesk/maya<version>/bin/plug-ins

Once installed, restart maya, you can now in the plugins manager load the plugin. 
To open the editor, simply run this code or press the bear button in the newly created BesEditor shelf:
   ```sh
cmds.showBesEditor()
   ```

  
</div>


