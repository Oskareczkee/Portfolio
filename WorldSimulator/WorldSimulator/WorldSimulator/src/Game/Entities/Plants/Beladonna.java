package Game.Entities.Plants;

import Game.Entities.Entity;
import Game.Entities.Organism;
import Game.Entities.Plant;
import Game.GamePanel;

public class Beladonna extends Plant {
    private String spritePath="Entities/Plants/belladonna.png";
    public Beladonna(GamePanel gp, int posX, int posY) {
        super(gp, posX, posY);
        loadSprite(spritePath);
        setMaxAge(25);
        setName("Beladonna");
    }

    @Override
    public boolean wasCollided(Entity entity)
    {
        if(entity instanceof Organism)
        {
            gp.gameWorld.killOrganism((Organism) entity);
            return false;
        }
        return true;
    }
}
