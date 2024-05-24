package Game.Entities.Animals;

import Game.Coords;
import Game.Entities.Animal;
import Game.Entities.Entity;
import Game.GamePanel;

import java.util.Random;

public class Sheep extends Animal {

    private String spritePath = "Entities/Animals/sheep.png";
    public Sheep(GamePanel gp, int posX, int posY) {
        super(gp, posX, posY);
        setStats(4,4);
        setMaxAge(50);
        loadSprite(spritePath);
        setName("Sheep");
    }
}
