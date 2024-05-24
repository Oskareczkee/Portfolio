package Game.Entities;

import Game.GamePanel;

import java.util.ArrayList;

public abstract class Organism extends Entity{
    public int Strength, Initiative, Age=0;
    private int MaxAge;

    //just for now cooldown time is 20, if needed can be set individually for each animal
    public int breedCooldownTime=20;
    public int breedCooldown=0;

    public Organism(GamePanel gp, int posX, int posY)
    {
        super(gp, posX, posY);
    }

    public void setStats(int Strength, int Initiative)
    {
        this.Strength=Strength;
        this.Initiative=Initiative;
    }

    public void setMaxAge(int maxAge)
    {
        this.MaxAge=maxAge;
    }
    public int getMaxAge(){return this.MaxAge;}

    @Override
    public void update(){}

    public void loadStats(ArrayList<Integer> stats)
    {
        Strength=stats.get(0);
        Initiative=stats.get(1);
        Age=stats.get(2);
        speed=stats.get(3);
        breedCooldownTime=stats.get(4);
    }

    public abstract AttackState attack(Organism defender);
    public abstract DefendState defend(Organism attacker);
}
