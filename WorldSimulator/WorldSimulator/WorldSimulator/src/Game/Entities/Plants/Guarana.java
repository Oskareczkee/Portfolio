package Game.Entities.Plants;

import Game.Entities.Entity;
import Game.Entities.Organism;
import Game.Entities.Plant;
import Game.GamePanel;

public class Guarana extends Plant {

    private String spritePath="Entities/Plants/guarana.png";
    public Guarana(GamePanel gp, int posX, int posY) {
        super(gp, posX, posY);
        loadSprite(spritePath);
        setMaxAge(20);
        setName("Guarana");
    }

    @Override
    public boolean wasCollided(Entity entity)
    {
        if(entity instanceof Organism)
        {
            Organism org = (Organism)entity;
            org.Strength+=3;
            gp.Console.log(entity.name+" Strength has been increased by 3");
            //guarana has been eaten
            gp.gameWorld.killOrganism(this);

            return false;
        }
        return true;
    }
}
