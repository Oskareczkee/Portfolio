package Game.Entities.Plants;

import Game.Entities.Plant;
import Game.GamePanel;

public class SowThistle extends Plant {

    private String spritePath="Entities/Plants/sowThistle.png";
    public SowThistle(GamePanel gp, int posX, int posY) {
        super(gp, posX, posY);
        loadSprite(spritePath);
        setMaxAge(5);
        setName("Sow Thistle");
    }

    @Override
    public void action()
    {
        //sow thistle tries to breed 3 times
        for(int x =0;x<3;x++)
            super.action();
    }
}
