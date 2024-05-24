package Game.Entities.Animals;

import Game.Coords;
import Game.Entities.Animal;
import Game.Entities.Organism;
import Game.GamePanel;

import java.util.Random;

public class Fox extends Animal {

    private String spritePath = "Entities/Animals/fox.png";
    public Fox(GamePanel gp, int posX, int posY) {
        super(gp, posX, posY);
        setStats(3,7);
        setMaxAge(70);
        loadSprite(spritePath);
        setName("Fox");
    }

    @Override
    public void action()
    {
        Random random = new Random();

        //we generate random numbers to perform moves, if both numbers are 0, there is no move
        int horizontalMove = random.ints(-1, 2).findFirst().getAsInt()*speed;
        int verticalMove = random.ints(-1, 2).findFirst().getAsInt()*speed;


        //if there was no move do not check colision
        if(horizontalMove==0 && verticalMove==0)
            return;

        var entity = gp.collisionChecker.getTileEntity(new Coords(x+ horizontalMove, y+ verticalMove));
        if(entity !=null && entity instanceof Organism)
        {
            Organism org = (Organism) entity;
            //if organism is stronger, do not move
            if(org.Initiative > this.Initiative)
                return;
            else if(org.Initiative==this.Initiative && org.Strength> this.Strength)
                return;
        }


        boolean movePossible = gp.collisionChecker.checkTile(this, x+ horizontalMove, y +verticalMove);
        if(!movePossible)
            return;

        x += horizontalMove;
        y += verticalMove;
    }
}
