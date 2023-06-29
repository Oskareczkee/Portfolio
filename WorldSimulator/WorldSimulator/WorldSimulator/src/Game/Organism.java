package Game;

public class Organism extends Entity{
    public int Strength, Initiative;
    public Organism(int posX, int posY, int str, int init)
    {
        super(posX, posY);
        loadSprite("boy_down_1.png");
        Strength=str;
        Initiative=init;
    }

    public Organism()
    {
        super(0,0);
        Strength=0;
        Initiative=0;
        loadSprite("boy_down_1.png");
    }
}
