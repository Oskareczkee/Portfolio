package Game.Entities.Animals;

import Game.Entities.Animal;
import Game.Entities.DefendState;
import Game.Entities.Entity;
import Game.Entities.Organism;
import Game.GamePanel;

import java.util.Random;

public class Turtle extends Animal {
    private String spritePath= "Entities/Animals/turtle.png";
    public Turtle(GamePanel gp, int posX, int posY) {
        super(gp, posX, posY);
        setStats(2,1);
        setMaxAge(350);
        loadSprite(spritePath);
        setName("Turtle");
    }

    //25% chance to move
    private double moveChance=0.25;

    @Override
    public void action() {
        Random random = new Random();
        if(!(random.nextDouble() < moveChance))
            return;

        super.action();
    }

    @Override
    public DefendState defend(Organism entity){
        if(entity.Strength < 5)
        {
            gp.Console.log("Turtle has refleted an attack from "+ entity.name);
            return DefendState.REFLECTED;
        }
        return DefendState.ATTACKED;
    }
}
