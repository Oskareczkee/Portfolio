package Game.Entities.Animals;

import Game.Entities.Animal;
import Game.Entities.Entity;
import Game.GamePanel;

public class Wolf extends Animal {
    private String spritePath= "Entities/Animals/wolf.png";
    public Wolf(GamePanel gp, int coordX, int coordY) {
        super(gp, coordX, coordY);
        setStats(9,5);
        setMaxAge(60);
        loadSprite(spritePath);
        setName("Wolf");
    }
}
