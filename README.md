<div align="center">

![Lagrange.Core.NativeAPI.Cpp](https://socialify.git.ci/kamenomi-dev/Lagrange.Core.NativeAPI.Cpp/image?custom_description=A+C+ABI-compatible+C%2B%2B+wrapper+for+the+Lagrange.Core.NativeAPI&description=1&font=JetBrains+Mono&forks=1&issues=1&language=1&logo=https%3A%2F%2Fstatic.live.moe%2Flagrange.jpg&name=1&owner=1&pattern=Plus&pulls=1&stargazers=1&theme=Dark)

</div>

## Introduction

​	This project implements the C++ interface of the `Lagrange.Core.NativeAPI ABI`, and encapsulates and integrates exported interface to make it easy to use.  
    ***Due to author's skill, this project supports Win32 platform only.***

## Usage

​	Step1. Download and extract the latest version, and place it in the `Include` or preferred path of the project.

​	Step2. Download and place `Lagrange.Core.NativeAPI.dll` from [LagrangeDev/LagrangeV2](https://github.com/LagrangeDev/LagrangeV2) in the root of project.

​	Then, have fun. :)



## Example

**Todo..., There is long to go**

## Appendix

### Disclaimer

​	This unofficial project is affiliated with the Lagrange project, which means that the disclaimer of this project is consistent with that of [LagrangeDev/LagrangeV2](https://github.com/LagrangeDev/LagrangeV2) and there is no deviation..

### Acknowledgement

1. Thanks to the Lagrange project for providing the `Lagrange.Core.NativeAPI.dll` and the C++ ABI, which is the basis of this project.
2. Thanks to nlohmann's JSON (MIT License) for providing function of parsing keystore file.
3. Thanks to spdlog (MIT License) for providing a simple and easy-to-use logging library.
   > I modified the import method of the header file inside to integrate it into the project according to the relative path in order to better integrate it into the project

## License

​	This project is open source under the GPL-3.0 license, which means you only need to declare the license and attach the link to this project where appropriate.

![img](https://www.gnu.org/graphics/gplv3-with-text-136x68.png)
