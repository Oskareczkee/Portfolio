package Game.Entities.Plants;

import Game.Coords;
import Game.Entities.Entity;
import Game.Entities.Organism;
import Game.Entities.Plant;
import Game.GamePanel;

public class SosnowkysHogweed extends Plant {
    private String spritePath="Entities/Plants/hogweed.png";
    public SosnowkysHogweed(GamePanel gp, int posX, int posY) {
        super(gp, posX, posY);
        loadSprite(spritePath);
        setMaxAge(25);
        setName("Sosnowsky's hogweed");
    }

    @Override
    public void action()
    {
        //get neighboring tiles
        Coords[] neighCells = gp.collisionChecker.getNeighboringCells(new Coords(this.x, this.y));

        //kill every organism in neighboring cells
        for (var coords : neighCells) {
            if (!gp.collisionChecker.isTileFree(coords))
            {
                var entity = gp.collisionChecker.getTileEntity(coords);
                if(entity instanceof Organism)
                {
                    Organism org = (Organism) entity;
                    gp.gameWorld.killOrganism(org);
                    gp.Console.log(this.name +" Terminated " + org.name);
                }

            }
        }

    }
}
