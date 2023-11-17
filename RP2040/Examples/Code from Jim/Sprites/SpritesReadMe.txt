Last updated: 02/16/23


Examples in this folder:

Sprites_01:
  This is an example of moving objects on the screen without sprites. Lots of flicker

Sprites_02:
  This is the Volos Project example of using Sprites_01 code modified for sprites. This code is not working 
  (dont know why!) 

Sprites_03:
  This is the Volos example of using transparent sprites. This does work, but it is slow. It demonstrates an arrow (62x54 pixels) created with a black background, moving acroww a purple screen. Also includes white text layered above the arrow.

Sprirites_04:
  This is an attempt to speed Sprites_03 up by making the arrow sprite smaller (30x26 pixels). Unfortunately, there is no speed improvement. I suspect that we are processor speed limited in how fast we can move our sprite.

Sprites_05:
  Improvement on sprites based on experiments and findings of Sprites_03. Here we abandon the    
canvasSprite and instead just write sprites directly to TFT display. This means we ONLY have to write 
a small number of pixels in the area of the sprite rather than the entire 320x240 screen.

Sprites_06:
  This is an extension of Sprites_03 to try to speed up the arrow by using an arrowSprite of size 2 pixels wider and taller than the size of the arrow graphic. If the background on the arrow image is the same color as the background, we SHOULD be able to move the arrowSprite around on the TFT (moves of 1 pixel in any direction) and not have to redraw the background (since the arrowSprite background will do that for free). Note we will not be able to use "transparent" features of sprites with this approach. We will use a black canvas for expediency since our current arrow image uses a black background. This works!

