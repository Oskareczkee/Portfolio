package Game;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;

public abstract class Entity implements Drawable{
    public int x=0, y=0;
    public int speed;
    public BufferedImage sprite;
    GamePanel gp;

    public Entity(int coordX, int coordY)
    {
        x=coordX; y=coordY;
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

    public void draw(Graphics2D g2) {
        try{
            g2.drawImage(sprite, x,y,gp.tileSize, gp.tileSize,null);
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }
    public void update(){}
}
