package Game;

import Game.Entities.Human;
import Game.Entities.Organism;

import java.awt.*;
import java.util.*;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class World{

    //we need concurrent collection in order to achieve safe read/insert while iterating world
    public Collection<Organism> organisms = new CopyOnWriteArrayList<>();
    private GamePanel gp;
    public void addOrganism(Organism organism)
    {
        organisms.add(organism);
    }
    public void killOrganism(Organism organism)
    {
        if(organism instanceof Human)
        {
            gp.Console.log("Player has been killed!");
            gp.playerAlive=false;
        }

        organisms.remove(organism);
    }

    public World(GamePanel gp)
    {
        this.gp=gp;
    }

    public void draw(Graphics2D g2)
    {
        for (var organism: organisms) {
            organism.draw(g2);
        }
    }


    //clears world out of entities, and prepares it for loading new ones
    public void clear()
    {
        organisms.clear();
    }

    public void update()
    {
        for(var organism : organisms)
        {
            organism.action();
            organism.Age++;
            if(organism.Age==organism.getMaxAge())
            {
                gp.Console.log(organism.name +" lost his soul with age");
                killOrganism(organism);
            }
        }
    }
}
