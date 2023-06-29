package Game;

import javax.swing.*;
import java.awt.*;

public class GamePanel extends JPanel implements Runnable
{
    final int originalTileSize = 16; // tiles are 16x16
    final int scale =3; //scaling for bigger resolutions
    public final int tileSize = originalTileSize * scale;
    final int maxScreenCol= 16;
    final int maxScreenRow = 12;
    final int screenWidth = tileSize * maxScreenCol; //768 px
    final int screenHeight = tileSize* maxScreenRow;//576 px

    //WORLD SETTINGS
    public final int maxWorldCol=50;
    public final int maxWorldRow=50;
    public final int worldWidth = tileSize * maxWorldCol;
    public final int worldHeight = tileSize * maxWorldRow;
    Thread gameThread;
    KeyHandler keyHandler = new KeyHandler();
    Human player = new Human(this, keyHandler);
    TileManager tileManger = new TileManager(this);
    World gameWorld = new World();
    int FrameRate = 60;
    public GamePanel()
    {
         this.setPreferredSize(new Dimension(screenWidth, screenHeight));
         this.setBackground(Color.BLACK);
         this.setDoubleBuffered(true);
         this.addKeyListener(keyHandler);
         this.setFocusable(true);

        Organism org = new Organism(96, 96, 2, 3);
        //org.loadSprite("boy_down_1.png");

        gameWorld.addOrganism(org);
    }

    public void startGameThread()
    {
        gameThread = new Thread(this);
        gameThread.start();
    }

    @Override
    public void run()
    {
        //this is basic game loop, it keeps the framerate, does not allow higher frame rates
        //higher framerates means more loops, and our character will move too fast
        //implemented delta method of loop, sleep method is also possible to implement

        double drawInterval = 1000/FrameRate; // 1 second is 10^3 milliseconds / fps
        double deltaTime=0;
        long lastTime = System.currentTimeMillis();
        long currentTime=0;
        long timer =0;
        int FPSCount =0;

        while(gameThread!=null) {

            currentTime = System.currentTimeMillis();
            deltaTime+=(currentTime-lastTime)/drawInterval;
            timer+=(currentTime-lastTime);
            lastTime = currentTime;

            if(deltaTime >=1)
            {
                update();
                repaint();
                deltaTime--;
                FPSCount++;
            }

            //timer hit 1 second, show current framerate
            if(timer>1000)
            {
                System.out.println("FPS: "+FPSCount);
                timer=0;
                FPSCount=0;
            }
        }
    }


    //TODO write here whole game loop, run keeps fps, but game should react only when player clicked the button
    public void update() {
        player.update();
    }

    public void paintComponent(Graphics g)
    {
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D) g;
        tileManger.draw(g2);
        gameWorld.draw(g2);
        player.draw(g2);
        g2.dispose();
    }

}
