package Game.Entities.Animals;

import Game.Coords;
import Game.Entities.Animal;
import Game.Entities.DefendState;
import Game.Entities.Entity;
import Game.Entities.Organism;
import Game.GamePanel;

import java.util.Random;

public class Antelope extends Animal {

    private String spritePath="Entities/Animals/antelope.png";
    public Antelope(GamePanel gp, int posX, int posY) {
        super(gp, posX, posY);
        setStats(4,4);
        setMaxAge(60);
        loadSprite(spritePath);
        setName("Antelope");
    }

    //50% chance to flee
    private double fleeChance =0.5;

    @Override
    public void action()
    {
        Random random = new Random();

        //we generate random numbers to perform moves, if both numbers are 0, there is no move
        //antelope has twice the movement range
        int horizontalMove = random.ints(-1, 2).findFirst().getAsInt()*2*speed;
        int verticalMove = random.ints(-1, 2).findFirst().getAsInt()*2*speed;


        //if there was no move do not check colision
        if(horizontalMove==0 && verticalMove==0)
            return;

        boolean movePossible = gp.collisionChecker.checkTile(this, x+ horizontalMove, y +verticalMove);
        if(!movePossible)
            return;

        x += horizontalMove;
        y += verticalMove;
    }

    //antelope flees to the nearest neighboring cell
    private void flee()
    {
        var neighCells = gp.collisionChecker.getFreeSurroundingTiles(new Coords(this.x, this.y));

        //nowhere to flee, stay in the same place
        if(neighCells.length==0)
            return;

        Random random = new Random();
        int num = random.ints(0, neighCells.length).findFirst().getAsInt();
        Coords tile = neighCells[num];

        this.x=tile.x;
        this.y=tile.y;
    }

    @Override
    public DefendState defend(Organism attacker)
    {
        Random random = new Random();
        if(random.nextDouble() <= fleeChance)
        {
            gp.Console.log("Antelope fleed the "+ attacker.name);
            flee();
            return DefendState.FLEE;
        }

        return DefendState.ATTACKED;
    }
}
