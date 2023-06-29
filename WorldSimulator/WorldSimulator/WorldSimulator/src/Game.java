import javax.swing.*;
import Game.GamePanel;


public class Game extends JFrame
{
    private JPanel mainPanel;
    private JTextPane consoleLog;
    private JPanel gamePanel;
    private JTextField textField1;
    private GamePanel Game;

    public Game()
    {
        setContentPane(mainPanel);
        setTitle("World simulator");
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setVisible(true);
        //setResizable(false);
        pack();
    }

    public static void main(String args[])
    {
        Game game = new Game();
        game.Game.startGameThread();
    }
}
