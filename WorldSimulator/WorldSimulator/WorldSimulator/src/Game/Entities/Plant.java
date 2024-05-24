package Game.Entities;

import Game.Coords;
import Game.GamePanel;

import java.util.Random;

public abstract class Plant extends Organism{
    public Plant(GamePanel gp, int posX, int posY) {
        super(gp, posX, posY);
        setStats(0,0);
    }

    //5% chance to breed
    public final double breedChance = 0.05;

    @Override
    public void action()
    {
        Random random = new Random();

        if(random.nextDouble() <= breedChance)
        {
            breed(this);
            gp.Console.log(this.name + " breeded!");
        }
    }

    public <T extends Plant> void breed(T plant)
    {
        var freeTiles = gp.collisionChecker.getFreeSurroundingTiles(new Coords(plant.x, plant.y));

        //no free tiles to breed
        if(freeTiles.length==0)
            return;

        Random random = new Random();
        int num = random.ints(0, freeTiles.length).findFirst().getAsInt();

        Coords tileCoords = freeTiles[num];
        try
        {
            var newEntity = plant.getClass().getConstructor(GamePanel.class, int.class, int.class).newInstance(gp, tileCoords.x, tileCoords.y);
            gp.gameWorld.addOrganism(newEntity);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    @Override
    public boolean wasCollided(Entity entity)
    {
        //if plant was collided its default behavior is to dissappear
        gp.Console.log(this.name + " has been eaten by " + entity.name);
        gp.gameWorld.killOrganism(this);
        return false;
    }
    @Override
    public boolean onCollision(Entity entity) {return true;}
    @Override
    public AttackState attack(Organism defender){return AttackState.KILLED;}
    @Override
    public DefendState defend(Organism attacker){return DefendState.ATTACKED;}
}
