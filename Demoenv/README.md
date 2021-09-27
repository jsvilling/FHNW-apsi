## Demo Environment
In certain lectures, demonstrations will be given. In others, you will have to work on practical exercises. Therefore, you will have to set up a virtual **Demo Environment**. In the following we describe two ways to do that. The easy way and the manual way.

### Setup the easy way
> Note that having already installed another virtualization solution, e.g., VMware may cause problems with VirtualBox and you should set up the Demo Environment manually using your existing virtualization solution!

#### Prerequisites
In order to set up the Demo Environment the easy way, you first need to install:

- [VirtualBox](https://www.virtualbox.org/wiki/Downloads)
- [Vagrant](https://www.vagrantup.com/downloads)

> On **Mac**, make sure you enable VirtualBox's kernel extension within 30 minutes after installation: **System Preferences → Security & Privacy → General**

> On **Windows 10**, to avoid problems, you should clone the repo outside of your user folder or any system folder, e.g., you could use a new folder *C:\FHNW*.
You should also create a new folder for the Vagrant boxes, e.g., *C:\Vagrant* and set the **$VAGRANT_HOME** enviroment variable to it by running the following command as an administrator:
`setx VAGRANT_HOME "C:/Vagrant" /M`

#### Install the Demo Environment with Vagrant
After you installed the necessary tools you may point your command line to this folder containing the `Vagrantfile` and hit the following command:`vagrant up`

> Note that you need a fast internet connection, since the linux distribution (Debian) will now be downloaded and set up.

#### Access the Demo Environment
At the end of the installation, you will see the hints on how to access the Demo Environment using SSH.

### Setup manually
If you choose to set up the Demo Environment manually, you will have to do the following with whatever tools you have in place.
Note that the vagrantfile is human-readable and lists most configuration commands directly.

The user account name with Vagrant is "vagrant". 
If you configure another user name, you may have to adjust this in some exercises.

#### Virtual machine
1. Get the latest Debian 11 "netinst" ISO file and set up a new virtual machine.
   
   Debian 11 ISO images are here: [Debian](https://www.debian.org/releases/bullseye/debian-installer/index)
1. Configure the virtual network (NAT) to forward the following TCP ports from the guest to the host machine: **22 → 2222, 80 → 80, 443 → 443, 8080 → 8080, 8081 → 8081, 9090 → 9090**.
   Make sure to put in 127.0.0.1 as Host IP or these ports may be accessible on your machine from outside.
1. Access your virtual machine using SSH on localhost using port 2222 and whatever authentication method you configured.
1. Install the following tools in the newly created virtual machine:
    - docker
    - build-essential
    - manpages-dev
    - gcc-multilib
    - gdb
    - git
    - curl
1. Execute the commands under "Install docker-compose" from the Vagrant-file
1. Clone this repo within your guest system in order to have the exercise material right in your Demo Environment using "git clone https://github.com/Fort-IT/FHNW-apsi.git" in your user directory.
    
