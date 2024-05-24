package Game.Entities;

import Game.Drawable;
import Game.GamePanel;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;

public abstract class Entity implements Drawable {
    public int x=0, y=0;
    public int speed;
    public BufferedImage sprite;

    //x and y are offsets of collision
    public Rectangle collision;
    public GamePanel gp;

    public String name;

    public Entity(GamePanel gp, int coordX, int coordY)
    {
        this.gp=gp;
        x=coordX; y=coordY;

        //set base collision to tile size, we do not need better one, can be improved for later games
        collision = new Rectangle(0,0 , gp.tileSize, gp.tileSize);
        //speed of 1 tile
        speed=gp.tileSize;
    }
    public void loadSprite(String path)
    {
        try {
            sprite = ImageIO.read(getClass().getClassLoader().getResourceAsStream(path));
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }

    public void setName(String name)
    {
        this.name=name;
    }

    public void draw(Graphics2D g2) {
        try{
            //draw entities that are in the player sight range
            //player has "camera" so we relate rendering to him
            int ScreenX = x - gp.player.x + gp.player.ScreenX;
            int ScreenY = y - gp.player.y + gp.player.ScreenY;



            //render only entities within the boundary of screen
            //render 1 entity more, to remove filckering when rendering
            if(x + gp.tileSize > gp.player.x - gp.player.ScreenX&&
                    x - gp.tileSize < gp.player.x + gp.player.ScreenX&&
                    y + gp.tileSize > gp.player.y -gp.player.ScreenY&&
                    y - gp.tileSize < gp.player.y + gp.player.ScreenY)
                g2.drawImage(sprite, ScreenX,ScreenY,gp.tileSize, gp.tileSize,null);
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }
    public abstract void update();
    public abstract void action();

    //returns if collision allows for move, can be modified easily
    //false means there should be no move, true move is possible
    public abstract boolean onCollision(Entity object);

    //Functions that allows to define behaviour what should happen
    //if some object collided with this entity
    //boolean returns whether any further actions should take place
    public abstract boolean wasCollided(Entity entity);
}
