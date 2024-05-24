package Game;

import javax.swing.*;

public class Logger {
    private JEditorPane consoleLog;

    public Logger(JEditorPane consoleLog)
    {
        this.consoleLog=consoleLog;
    }
    public void log(String message)
    {
        var doc = consoleLog.getDocument();

        try {
            doc.insertString(doc.getLength(), message +"\n", null);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

        //autoscrolling
        consoleLog.setCaretPosition(consoleLog.getText().length());
    }
}
