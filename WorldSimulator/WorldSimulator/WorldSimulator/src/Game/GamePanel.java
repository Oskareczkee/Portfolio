package Game;

import Game.Entities.Animals.*;
import Game.Entities.CollisionChecker;
import Game.Entities.Human;
import Game.Entities.Plants.*;
import Game.Handlers.KeyHandler;
import Game.Handlers.TileManager;

import javax.swing.*;
import java.awt.*;

public class GamePanel extends JPanel implements Runnable
{
    final int originalTileSize = 16; // tiles are 16x16
    final int scale =3; //scaling for bigger resolutions
    public final int tileSize = originalTileSize * scale;
    public final int maxScreenCol= 16;
    public final int maxScreenRow = 12;
    public final int screenWidth = tileSize * maxScreenCol; //768 px
    public final int screenHeight = tileSize* maxScreenRow;//576 px

    //WORLD SETTINGS
    public final int maxWorldCol=50;
    public final int maxWorldRow=50;
    public final int worldWidth = tileSize * maxWorldCol;
    public final int worldHeight = tileSize * maxWorldRow;
    Thread gameThread;
    KeyHandler keyHandler = new KeyHandler();
    public Human player = new Human(this, keyHandler);

    //simple gamestate boolean, just to know when to end game
    public boolean playerAlive=true;

    //gamestate boolean, to know if player performed an action, used for update
    public boolean playerAction=false;
    TileManager tileManger = new TileManager(this);
    public Logger Console;
    public CollisionChecker collisionChecker = new CollisionChecker(this);
    public SaveLoadTool saveLoadTool= new SaveLoadTool(this);
    public World gameWorld = new World(this);
    int FrameRate = 2;
    public GamePanel()
    {
         this.setPreferredSize(new Dimension(screenWidth, screenHeight));
         this.setBackground(Color.BLACK);
         this.setDoubleBuffered(true);
         this.addKeyListener(keyHandler);
         this.setFocusable(true);
         setupGameWorld();
    }

    private void setupGameWorld()
    {
        player.x= 288;
        player.y = 288;

        Sheep sheep = new Sheep(this, 576,48);
        Sheep sheep1 = new Sheep(this, 144,144);
        Fox fox = new Fox(this, 288,384);
        Fox fox1 = new Fox(this, 288,432);
        Antelope antelope = new Antelope(this, 288,0);
        Antelope antelope1 = new Antelope(this, 144,48);
        Turtle turtle = new Turtle(this, 0 ,0);
        Turtle turtle1 = new Turtle(this, 48 ,0);
        Wolf wolf = new Wolf(this, 384,144);
        Wolf wolf1 = new Wolf(this, 432,144);
        Grass grass = new Grass(this, 96, 96);
        Grass grass1 = new Grass(this, 1440, 1440);
        SowThistle st = new SowThistle(this, 768, 768);
        SowThistle st1 = new SowThistle(this, 624, 624);
        SosnowkysHogweed sh = new SosnowkysHogweed(this, 144, 192);
        SosnowkysHogweed sh1 = new SosnowkysHogweed(this, 1440, 1920);
        Beladonna bd = new Beladonna(this, 240, 192);
        Beladonna bd1 = new Beladonna(this, 240*5, 192*3);
        Guarana gu = new Guarana(this, 384, 288);
        Guarana gu1 = new Guarana(this, 384*4, 288*2);



        gameWorld.addOrganism(sheep);
        gameWorld.addOrganism(sheep1);
        gameWorld.addOrganism(fox);
        gameWorld.addOrganism(fox1);
        gameWorld.addOrganism(antelope);
        gameWorld.addOrganism(antelope1);
        gameWorld.addOrganism(turtle);
        gameWorld.addOrganism(turtle1);
        gameWorld.addOrganism(wolf1);
        gameWorld.addOrganism(wolf);
        gameWorld.addOrganism(grass);
        gameWorld.addOrganism(grass1);
        gameWorld.addOrganism(st);
        gameWorld.addOrganism(st1);
        gameWorld.addOrganism(sh);
        gameWorld.addOrganism(sh1);
        gameWorld.addOrganism(bd);
        gameWorld.addOrganism(bd1);
        gameWorld.addOrganism(gu);
        gameWorld.addOrganism(gu1);
        gameWorld.addOrganism(player);
    }

    public void startGameThread()
    {
        gameThread = new Thread(this);
        gameThread.start();
    }

    public void setupConsole(JEditorPane pane)
    {
        Console = new Logger(pane);
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
                //Console.log("FPS: " + FPSCount);
                timer=0;
                FPSCount=0;
            }
        }
    }


    //TODO write here whole game loop, run keeps fps, but game should react only when player clicked the button
    public void update() {

        if(keyHandler.loadGamePressed)
            saveLoadTool.loadWorldState("savegame.dat");

        if(keyHandler.saveGamePressed)
            saveLoadTool.saveWorldState("savegame.dat");

        //update world only if player is alive
        if(playerAlive)
        {
            player.update();
            if(playerAction) //if player performed any action, update world
                gameWorld.update();
            playerAction=false; //reset state
        }
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
