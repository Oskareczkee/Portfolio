package Game.Entities.Plants;

import Game.Entities.Entity;
import Game.Entities.Plant;
import Game.GamePanel;

public class Grass extends Plant {
    private String spritePath="Entities/Plants/grassPlant.png";
    public Grass(GamePanel gp, int posX, int posY) {
        super(gp, posX, posY);
        loadSprite(spritePath);
        setMaxAge(15);
        setName("Grass");
    }
}
