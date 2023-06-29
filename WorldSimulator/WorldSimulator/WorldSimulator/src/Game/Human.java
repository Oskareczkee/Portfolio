package Game;

import java.awt.*;

//Also known as Player class, project requires to name it Human
public class Human extends Organism{
    GamePanel gp;
    KeyHandler keyHandler;
    public final int ScreenX, ScreenY;
    public String spritePath="boy_down_1.png";

    public Human(GamePanel gp, KeyHandler kh)
    {
        this.gp=gp;
        this.keyHandler=kh;
        loadSprite(spritePath);

        //set the player in the center of the camera - tileSize/2, to adujst properly camera
        ScreenX = gp.screenWidth/2 - (gp.tileSize/2);
        ScreenY = gp.screenHeight/2 - (gp.tileSize/2);

        x=96;
        y=96;
        speed=4;
    }


    public void update(){
        if(keyHandler.upPressed)
            y-=speed;
        if(keyHandler.downPressed)
            y+=speed;
        if(keyHandler.rightPressed)
            x+=speed;
        if(keyHandler.leftPressed)
            x-=speed;
    }

    public void draw(Graphics2D g2){
        g2.drawImage(sprite, ScreenX,ScreenY,gp.tileSize, gp.tileSize,   null);
    }

}
