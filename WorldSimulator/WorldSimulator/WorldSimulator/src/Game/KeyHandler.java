package Game;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
public class KeyHandler implements KeyListener
{
    //KEY BINDS
    public enum KEY_BINDS
    {
        UP (KeyEvent.VK_W),
        DOWN(KeyEvent.VK_S),
        LEFT(KeyEvent.VK_A),
        RIGHT(KeyEvent.VK_D),
        UP_ARROW(KeyEvent.VK_UP),
        DOWN_ARROW(KeyEvent.VK_DOWN),
        LEFT_ARROW(KeyEvent.VK_LEFT),
        RIGHT_ARROW(KeyEvent.VK_RIGHT);

        int keyCode;
        private KEY_BINDS(int code)
        {
            keyCode=code;
        }

    }

    public boolean upPressed=false,
                   downPressed=false,
                   rightPressed=false,
                   leftPressed=false;


    @Override
    public void keyTyped(KeyEvent e) {

    }

    @Override
    public void keyPressed(KeyEvent e) {
        int code = e.getKeyCode();

        if(code == KEY_BINDS.UP.keyCode || code == KEY_BINDS.UP_ARROW.keyCode)
            upPressed=true;
        if(code == KEY_BINDS.DOWN.keyCode || code == KEY_BINDS.DOWN_ARROW.keyCode)
            downPressed=true;
        if(code == KEY_BINDS.RIGHT.keyCode || code == KEY_BINDS.RIGHT_ARROW.keyCode)
            rightPressed=true;
        if(code == KEY_BINDS.LEFT.keyCode || code == KEY_BINDS.LEFT_ARROW.keyCode)
            leftPressed=true;
    }

    @Override
    public void keyReleased(KeyEvent e) {
        int code = e.getKeyCode();

        if(code == KEY_BINDS.UP.keyCode || code == KEY_BINDS.UP_ARROW.keyCode)
            upPressed=false;
        if(code == KEY_BINDS.DOWN.keyCode || code == KEY_BINDS.DOWN_ARROW.keyCode)
            downPressed=false;
        if(code == KEY_BINDS.RIGHT.keyCode || code == KEY_BINDS.RIGHT_ARROW.keyCode)
            rightPressed=false;
        if(code == KEY_BINDS.LEFT.keyCode || code == KEY_BINDS.LEFT_ARROW.keyCode)
            leftPressed=false;
    }
}
