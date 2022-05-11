[TOC]

# 1.Basic laws of geometrical optics and imaging concepts

- The electromagnetic spectrum
  - wave length: short -> long
  - GammaRays | X-Rays | UltraViolet | VisibleLight(380nm-780nm) | InfraRed | MicroWaves | RadioWaves

## 1.1Basic laws

1. **Law of Rectilinear propagation**: Light -> free space / homogeneous, isotropic matter = beeline.
2. **Law of independent propagation**: different lights meet, not affect each other
3. **Law of Reflection**: Light -> reflecting surface and reflected make equal angles
   1. $I''=-I$
   2. incoming, outgoing, normal $\in$ same plane
   3. **Total Internal Reflection**![在这里插入图片描述](https://img-blog.csdnimg.cn/c07be9a25b1c40bf9510b53bb9035549.png)


4. **Law of Refraction**: incident angle ~ normal ~ refracted angle
   1. $n\sin I=n'\sin I'$
   2. incident, refracted, normal $\in$ same plane

- **Fermat' principle**: light takes the path that requires the least time
- **Malus' principle**: In homogeneous&isotropic matter, Light $\perp$ WaveFront

## 1.2Light path calculation

![在这里插入图片描述](https://img-blog.csdnimg.cn/292824949b8545f6846ea6643e0ec975.png)

Calculate light path
- $\frac{\sin I}{r-L}=\frac{\sin-U}{r}$
- $n'\sin I'=n\sin I$
- $U'+I'=U+I$
- $\frac{\sin I'}{L'-r}=\frac{\sin U'}{r}$

Paraxial rays
- $i=\frac{l-r}{r}u$
- $i'=\frac{n}{n'}i$
- $u'=u+i-i'$
- $l'=r(1+\frac{i'}{u'})$
- $l'u'=lu=h$
- $\frac{n'}{l'}-\frac{n}{l}=\frac{n'-n}{r}$

## 1.3Imaging
Magnification
- Transverse magnification: $\beta=\frac{y'}{y}=\frac{n}{n'}\frac{l'}{l}$
- Axial magnification: $\alpha=\frac{dl'}{dl}=\frac{n'}{n}\beta^2$
- Angular magnification: $\gamma=\frac{\tan U'}{\tan U}=\frac{n}{n'}\frac{1}{\beta}$
- $\alpha\gamma=\beta$
- Lagrange's invariant: $J=n'u'y'=nuy$
- Reflection: $n=-n',\frac{1}{l'}+\frac{1}{l}=\frac{2}{r}$

Image characteristics
- larger | smaller
- upright | inverted
- real | virtual

### homework

1. A staff 2m long when held erect casts a shadow 3.4 m long, while a building’s shadow is 170 m long. How tall is the building? 
   1. similar triangles:$\frac{x}{170}=\frac{2}{3.4},x=100m$
2. Light from a water medium with n=1.33 is incident upon a water-glass interface at an angle of 45$^\circ$ . The glass index is 1.50. What angle does the light make with the normal in the glass?
   1. Law of refraction:$1.33\sin45^\circ=1.5\sin\theta,\theta=38.83^\circ$
3. A goldfish swims 10cm from the side of a spherical bowl of water of radius 20cm. Where does the fish appear to be? Does it appear larger or smaller?
   1. $\frac{n'}{l'}-\frac{n}{l}=\frac{n'-n}{r},l'=-8.58cm$
   2. $\beta=\frac{n}{n'}\frac{l'}{l},\beta=1.14>1$,bigger
4. An object is located 2cm to the left of convex end of a glass rod which has a radius of curvature of 1cm. The index of refraction of the glass is n=1.5. Find the image distance.
   1. $\frac{n'}{l_1'}-\frac{n}{l_1}=\frac{n'-n}{r_1},l_1'=-\infty$
   2. $\frac{n}{l_2'}-\frac{n'}{l_2}=\frac{n-n'}{r_2},l_2'=2cm$
   
# 2.Perfect optical system

## 2.1Concept

![在这里插入图片描述](https://img-blog.csdnimg.cn/6e41e7ce1a2040589623e2a2ac819591.png)

- Principal plane: conjugate planes, $\beta=\pm 1$
- Focal plane
   $\frac{f'}{f}=-\frac{n'}{n}$
- Nodal plane: conjugate planes, $\gamma=\pm 1$
   $x_J=f', x_J'=f$
- Nodal points: no refraction occurs
  - n=n', NodalPoints=PrincipalPoints
  - n$\neq$n', NodalPoints->bigger n side

## 2.2Grqphical construction
![在这里插入图片描述](https://img-blog.csdnimg.cn/c36435a13cb44f668c0319f32e4478e4.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/b373eae337374ffeb272289498e11658.png)

## 2.3Image position fomula

- Newton's equation: $xx'=ff'$
- Gauss' equation: $\frac{f'}{l'}+\frac{f}{l}=1$,same matter$\frac{1}{l'}-\frac{1}{l}=\frac{1}{f'}$

## 2.4Magnificaation
- $\beta=-\frac{f}{x}=-\frac{x'}{f'}$
- $\alpha=-\frac{x'}{x}=-\frac{f'}{f}\beta$
- $\gamma=\frac{n}{n'}\frac{1}{\beta}$
- $\alpha\gamma=\beta$

## 2.5Combination

![在这里插入图片描述](https://img-blog.csdnimg.cn/91fcf7501433448a9c24a366605593be.png)
- $f'=-\frac{f_1'f_2'}{\Delta}$ ~ $f=\frac{f_1f_2}{\Delta}$
- $l_F'=f'(1-\frac{d}{f_1'})$ ~ $l_F=f(1+\frac{d}{f_2})$
- $l_H'=-f'\frac{d}{f_1'}$ ~ $l_H=f\frac{d}{f_2}$
- focal power: $\varPhi=\varPhi_1+\varPhi_2-d\varPhi_1\varPhi_2$

## 2.6examples
Telephoto group
![在这里插入图片描述](https://img-blog.csdnimg.cn/6d8973f0a965413fa6d1de7e0d5289a1.png)
- used for long focal length lenses to reduce mechanical dimensions

Inverse telephoto group
![在这里插入图片描述](https://img-blog.csdnimg.cn/93cd909781b64dff880b7128e57a5795.png)
- used for short focal length & long work length(microscope objective)

## 2.7Lens
Convex lens not always are plus lens;
Concave lens not always are negative lens.
in air: $f'=\frac{nr_1r_2}{(n-1)[n(r_2-r_1)+(n-1)d]}$
thin lens d->0: $\varPhi=(n-1)(\rho_1-\rho_2)$

## homework
1. An object 1cm high is 30cm in front of a thin lens with a focal length of 10cm. Where is the image? Verify your answer by graphical construction of the image.
2. A lens is known to have a focal length of 30cm in air. An object is placed 50cm to the left of the lens. Locate the image.
3. The object is a transparent cube, 4mm across, placed 60cm in front of a lens of 20cm focal length. Calculate the transverse and axial magnification and describe what the image looks like?

