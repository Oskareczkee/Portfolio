package Game.Entities;

import Game.Coords;
import Game.GamePanel;

import java.util.Random;

public abstract class Animal extends Organism {
    public Animal(GamePanel gp, int posX, int posY) {
        super(gp, posX, posY);
    }

    @Override
    public void action() {

        //only animals use breedCooldown
        if(breedCooldown > 0)
            breedCooldown--;

        Random random = new Random();

        //we generate random numbers to perform moves, if both numbers are 0, there is no move
        int horizontalMove = random.ints(-1, 2).findFirst().getAsInt()*speed;
        int verticalMove = random.ints(-1, 2).findFirst().getAsInt()*speed;


        //if there was no move do not check colision
        if(horizontalMove==0 && verticalMove==0)
            return;

        boolean movePossible = gp.collisionChecker.checkTile(this, x+ horizontalMove, y +verticalMove);

        if(!movePossible)
            return;

        x += horizontalMove;
        y += verticalMove;
    }

    public <T extends Animal> void breed(T entity)
    {
        var freeTiles = gp.collisionChecker.getFreeSurroundingTiles(new Coords(entity.x, entity.y));

        //no free tiles to breed
        if(freeTiles.length==0)
            return;

        Random random = new Random();

        int num = random.ints(0, freeTiles.length).findFirst().getAsInt();

        Coords tileCoords = freeTiles[num];
        try
        {
            var newEntity = entity.getClass().getConstructor(GamePanel.class, int.class, int.class).newInstance(gp, tileCoords.x, tileCoords.y);
            //give this new entity breed cooldown as well
            newEntity.breedCooldown=breedCooldownTime;
            gp.gameWorld.addOrganism(newEntity);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

    }
    @Override
    public AttackState attack(Organism defender)
    {
        Organism organism = (Organism) defender;

        //fight starts organism with higher initiative
        if(this.Initiative == organism.Initiative)
        {
            if(this.Strength==organism.Strength)
            {
                var state = organism.defend(this);
                //attack has been reflected
                if(state==DefendState.REFLECTED)
                    return AttackState.REFLECTED;
                else if (state==DefendState.FLEE)
                    return AttackState.UNSUCCESSFUL;
                gp.gameWorld.killOrganism(organism); //iniciator of fight wins
            }
            else if(this.Strength > organism.Strength)
            {
                var state = organism.defend(this);
                //attack has been reflected
                if(state==DefendState.REFLECTED)
                    return AttackState.REFLECTED;
                else if (state==DefendState.FLEE)
                    return AttackState.UNSUCCESSFUL;
                gp.gameWorld.killOrganism(organism);
            }
        }
        else if (this.Initiative > organism.Initiative) {
            var state = organism.defend(this);
            //attack has been reflected
            if(state==DefendState.REFLECTED)
                return AttackState.REFLECTED;
            else if (state==DefendState.FLEE)
                return AttackState.UNSUCCESSFUL;
            gp.gameWorld.killOrganism(organism);
        }
        //attacker is too week
        else {
            return  AttackState.UNSUCCESSFUL;
        }

        if(defender instanceof Animal)
            gp.Console.log(defender.name +" has been killed by "+ this.name);

        return AttackState.KILLED;
    }

    //basic animals do not know how to defend, turtle does
    @Override
    public DefendState defend(Organism attacker) {return DefendState.ATTACKED;}

    @Override
    public boolean wasCollided(Entity entity){return true;}
    @Override
    public boolean onCollision(Entity entity)
    {
        if(this.getClass().getName().equals(entity.getClass().getName()))
        {
            if(this.breedCooldown != 0 && ((Organism) entity).breedCooldown!=0)
                return false;

            breed(this);
            gp.Console.log("New baby " + this.name +" was born");

            //set breeding cooldown
            this.breedCooldown=this.breedCooldownTime;
            ((Organism) entity).breedCooldown=((Organism) entity).breedCooldownTime;

            return false;
        }
        //if entity is organism, check collision (not implemented here, we do not have colliding blocks etc.)
        if(!(entity instanceof Organism))
            return true;

        //call wasCollided if entity has some special abilities (like guarana)
        //should fight take place, if not return
        boolean nextActions = entity.wasCollided(this);
        if(!nextActions)
            return true;


        //entities both attack theirselves
        var attackState = attack((Organism) entity);

        //if entity has been killed, it should not respond
        if(!(attackState==AttackState.KILLED))
            ((Organism) entity).attack(this);

        //after reflected attack attacker should stay at his place
        if(attackState==AttackState.REFLECTED)
            return false;

        //if attack was not reflected (defender was killed or defender flee)
        return true;
    }

    @Override
    public void update(){}
}
