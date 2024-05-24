import javax.swing.*;
import Game.GamePanel;
import Game.Logger;


public class Game extends JFrame
{

    //generated automatically by intelliJ
    private JPanel mainPanel;
    private JPanel gamePanel;
    private GamePanel Game;
    private JScrollPane LogPanel;
    private JEditorPane Log;

    public Game()
    {
        setContentPane(mainPanel);
        setTitle("World simulator");
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setVisible(true);
        setResizable(false);
        pack();

    }

    public static void main(String args[])
    {
        Game game = new Game();
        game.Game.startGameThread();
        game.Game.setupConsole(game.Log);
    }
}
