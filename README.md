
# PIgate - Plate Recognition Gate Using Raspberry PI 4





## Appendix


Within the scope of embedded systems master’s course was proposed to do a project which will apply all the taught subjects, like threads, TCP-IP, daemons, etc.

The goal of the project is not to replace remotes altogether but rather to complement them. The user will still be able to open the gate as always, but this product will facilitate the process of getting home. Thus, no need to develop a gate from scratch.

The chosen development board was the Raspberry PI 4. It will be running a costume Linux image generated with buildroot.


## Installation

To generate the costum Linux image download the buildroot tool

```bash
https://buildroot.org/download.html
```
    
To have your PIgate database, create a Firebase account and modify the Firebase configs, which are present in firebase modules.

After connecting the raspberry with the costume Linux image, type make in the terminal in the local system folder and it should send all the necessary files. NOTE: verify your Raspberry IP address.

To make the wifi and camera module don't forget to follow the step present in the builroot folder.




## Documentation

Documentation to the Local System code is present in the LocalSystem/docs/Doxygen

Project report is also available.
## Authors

- [@Hugo Ribeiro](https://github.com/HugoRibeiro-A88287-UM)
- [@Ricardo Mendes](https://github.com/Portagens)

