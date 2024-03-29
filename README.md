> For this is how God loved the world:  
he gave his only Son, so that everyone  
who believes in him may not perish  
but may have eternal life.  
  \
John 3:16

## The Library

I started to code an opensource library called OpenGLStarter.

The code become so big, that I decided to split it into independent libraries.

I splitted the code according its area.

For example: The math and basic utilities I called aRibeiroCore; The platform related code (thread, sleep, timer, etc...) I called aRibeiroPlatform, etc...

I have a WIP documentation at the [docs](docs/index.md) folder.

I hope this code could help/aid the development of great projects.

## How to Clone?

This library uses git submodules.

You need to fetch the repo and the submodules also.

### a) Clone With Single Command

__HTTPS__

```bash
git clone --recurse-submodules https://github.com/A-Ribeiro/aRibeiroData.git
```

__SSH__

```bash
git clone --recurse-submodules git@github.com:A-Ribeiro/aRibeiroData.git
```

### b) Clone With Multiple Commands

__HTTPS__

```bash
git clone https://github.com/A-Ribeiro/aRibeiroData.git
cd OpenGLStarter
git submodule init
git submodule update
```

__SSH__

```bash
git clone git@github.com:A-Ribeiro/aRibeiroData.git
cd OpenGLStarter
git submodule init
git submodule update
```

## Authors

***Alessandro Ribeiro da Silva*** obtained his Bachelor's degree in Computer Science from Pontifical Catholic 
University of Minas Gerais and a Master's degree in Computer Science from the Federal University of Minas Gerais, 
in 2005 and 2008 respectively. He taught at PUC and UFMG as a substitute/assistant professor in the courses 
of Digital Arts, Computer Science, Computer Engineering and Digital Games. He have work experience with interactive
software. He worked with OpenGL, post-processing, out-of-core rendering, Unity3D and game consoles. Today 
he work with freelance projects related to Computer Graphics, Virtual Reality, Augmented Reality, WebGL, web server 
and mobile apps (andoid/iOS).

More information on: https://alessandroribeiro.thegeneralsolution.com/en/
