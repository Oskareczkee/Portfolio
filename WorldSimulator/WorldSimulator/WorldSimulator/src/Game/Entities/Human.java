package Game.Entities;

import Game.Coords;
import Game.GamePanel;
import Game.Handlers.KeyHandler;

import java.awt.*;

//Also known as Player class, project requires to name it Human
public class Human extends Organism {
    KeyHandler keyHandler;
    public final int ScreenX, ScreenY;
    public String spritePath= "Entities/player.png";
    private final int specialAbilityCooldownTime = 10;
    public int specialAbilityCooldown= 0;

    public Human(GamePanel gp, KeyHandler kh)
    {
        super(gp, 96,96);
        setStats(5,4);
        //human does not age for gameplay purposes
        setMaxAge(-1);
        this.keyHandler=kh;
        loadSprite(spritePath);
        setName("Human");

        //set the player in the center of the camera - tileSize/2, to adujst properly camera
        ScreenX = gp.screenWidth/2 - (gp.tileSize/2);
        ScreenY = gp.screenHeight/2 - (gp.tileSize/2);
    }

    private void abilityPurification()
    {
        if(specialAbilityCooldown!=0)
        {
            gp.Console.log("You can use your special ability in " + specialAbilityCooldown + " turns");
            return;
        }

        var coords = gp.collisionChecker.getNeighboringCells(new Coords(this.x, this.y));

        for(var tileCoord : coords)
        {
            var entity = gp.collisionChecker.getTileEntity(tileCoord);
            if(entity!=null)
                if(entity instanceof Organism)
                {
                    gp.Console.log(entity.name + " has been incinerated and sent to final judgment by " + this.name);
                    gp.gameWorld.killOrganism((Organism) entity);
                }
        }

        specialAbilityCooldown+=specialAbilityCooldownTime;
    }

    public void update(){
        int newCoordX=x, newCoordY=y;

        if(keyHandler.upPressed)
            newCoordY-=speed;
        if(keyHandler.downPressed)
            newCoordY+=speed;
        if(keyHandler.rightPressed)
            newCoordX+=speed;
        if(keyHandler.leftPressed)
            newCoordX-=speed;
        if(keyHandler.specialAbilityPressed)
        {
            //player performed action - used ability
            abilityPurification();
            gp.playerAction=true;
        }


        //there was no move, do not check collision
        if(newCoordX==this.x && newCoordY==this.y)
            return;

        //player performed action, a move
        gp.playerAction=true;

        var movePossible = gp.collisionChecker.checkTile(this, newCoordX, newCoordY);

        //move was not possible collision or some event occured
        if(!movePossible)
            return;

        x=newCoordX;
        y=newCoordY;

        if(specialAbilityCooldown!=0)
            specialAbilityCooldown--;
    }

    @Override
    public void action() {

    }

    @Override
    public boolean onCollision(Entity entity) {
        //if entity is organism, check collision (not implemented here, we do not have colliding blocks etc.)
        if(!(entity instanceof Organism))
            return true;

        //call wasCollided if entity has some special abilities (like guarana)
        //should fight take place, if not return
        boolean nextActions = entity.wasCollided(this);
        if(!nextActions)
            return true;

        //call wasCollided if entity has some special abilities
        entity.wasCollided(this);

        var attackState = attack((Organism) entity);
        if(!(attackState==AttackState.KILLED))
            ((Organism) entity).defend(this);

        //if player attack was reflected he should stay at his place
        if(attackState==AttackState.REFLECTED)
            return false;
        return true;
    }

    @Override
    public boolean wasCollided(Entity entity) {return true;}

    public void draw(Graphics2D g2){
        g2.drawImage(sprite, ScreenX,ScreenY,gp.tileSize, gp.tileSize,   null);
    }

    @Override
    public AttackState attack(Organism defender) {
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
        //human is too week
        else {
            return  AttackState.UNSUCCESSFUL;
        }

        if(defender instanceof Animal)
            gp.Console.log(defender.name +" has been killed by "+ this.name);

        return AttackState.KILLED;
    }

    //attack is the best form of defence
    @Override
    public DefendState defend(Organism attacker) {return DefendState.ATTACKED;}
}
