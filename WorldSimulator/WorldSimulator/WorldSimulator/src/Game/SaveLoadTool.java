package Game;

import Game.Entities.Human;
import Game.Entities.Organism;

import java.io.*;
import java.util.ArrayList;

public class SaveLoadTool {
    GamePanel gp;

    public SaveLoadTool(GamePanel gp)
    {
        this.gp=gp;
    }
    public void loadWorldState(String filePath)
    {
        //clear the world before inserting new organisms
        gp.gameWorld.clear();

        try {
            ObjectInputStream ois = new ObjectInputStream(new FileInputStream(new File(filePath)));
            SaveData sd = (SaveData) ois.readObject();

            gp.player.x=sd.playerX;
            gp.player.y=sd.playerY;
            gp.player.specialAbilityCooldown=sd.specialAbilityCooldown;
            gp.player.Initiative=sd.playerInitiative;
            gp.player.Strength=sd.playerStrength;

            //we add player to the gameworld
            gp.gameWorld.addOrganism(gp.player);
            gp.playerAlive=sd.playerAlive;

            for(int x =0;x<sd.organismsClassNames.size();x++)
            {
                int coordX = sd.organismsCoords.get(x).get(0);
                int coordY = sd.organismsCoords.get(x).get(1);

                var objClass = Class.forName(sd.organismsClassNames.get(x))
                        .getConstructor(GamePanel.class, int.class, int.class)
                        .newInstance(this.gp, coordX, coordY);

                if(objClass instanceof Organism)
                {
                    Organism organism = (Organism) objClass;

                    organism.loadStats(sd.organismsStats.get(x));
                    gp.gameWorld.addOrganism(organism);
                }
                //we do not have yet no organism entities, so nothing is done here :(
            }


        }
        catch (Exception e){
            e.printStackTrace();
        }

        gp.Console.log("Game has been loaded");
    }

    public void saveWorldState(String fileName)
    {
        try{
            ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(new File(fileName)));
            SaveData sd = new SaveData();

            sd.playerX=gp.player.x;
            sd.playerY=gp.player.y;
            sd.specialAbilityCooldown=gp.player.specialAbilityCooldown;
            sd.playerInitiative=gp.player.Initiative;
            sd.playerStrength=gp.player.Strength;

            sd.playerAlive=gp.playerAlive;

            for(var entity : gp.gameWorld.organisms)
            {
                //skip player, he has been handled
                if(entity instanceof Human)
                    continue;

                sd.organismsClassNames.add(entity.getClass().getName());
                ArrayList<Integer> organismStats = new ArrayList<>();
                ArrayList<Integer> organismCoords = new ArrayList<>();
                organismCoords.add(entity.x);
                organismCoords.add(entity.y);

                organismStats.add(entity.Strength);
                organismStats.add(entity.Initiative);
                organismStats.add(entity.Age);
                organismStats.add(entity.speed);
                organismStats.add(entity.breedCooldown);

                sd.organismsStats.add(organismStats);
                sd.organismsCoords.add(organismCoords);
            }

            oos.writeObject(sd);
        }
        catch (Exception e){
            e.printStackTrace();
        }

        gp.Console.log("Game has been saved");
    }

    //CAN BE DONE LATER: load maps (and maybe save if map can get changed dynamically)


}
