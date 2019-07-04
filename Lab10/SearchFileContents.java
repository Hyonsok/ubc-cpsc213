import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.*;

public class SearchFileContents extends JFrame {

    private DefaultListModel<File> fileListModel;
    private JList<File> fileList;
    private JLabel searchTextLabel;
    private JTextField searchTextField;
    private JLabel directoryLabel;
    private JTextField directoryTextField;
    private JButton directorySelectButton;
    private JButton startSearchButton;
    private JButton stopButton;
    private JLabel resultCountLabel;

    private boolean searchInProgress = false;
    private boolean searchInterrupted = false;

    public SearchFileContents() {
        super("Search File Contents");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setSize(800, 600);

        JPanel searchTextPanel = new JPanel(new FlowLayout());
        this.getContentPane().add(searchTextPanel, BorderLayout.NORTH);
        JPanel part2 = new JPanel(new BorderLayout());
        this.getContentPane().add(part2, BorderLayout.CENTER);
        JPanel directoryPanel = new JPanel(new FlowLayout());
        part2.add(directoryPanel, BorderLayout.NORTH);
        JPanel part3 = new JPanel(new BorderLayout());
        part2.add(part3, BorderLayout.CENTER);
        JPanel buttons = new JPanel(new FlowLayout());
        part3.add(buttons, BorderLayout.NORTH);
        // TODO Add list to part 3

        searchTextLabel = new JLabel("Search files with text:");
        searchTextPanel.add(searchTextLabel);

        searchTextField = new JTextField(40);
        searchTextLabel.setLabelFor(searchTextField);
        searchTextPanel.add(searchTextField);

        directoryLabel = new JLabel("In directory:");
        directoryPanel.add(directoryLabel);

        String currentDirectory = null;
        try {
            currentDirectory = new File(".").getCanonicalPath();
        } catch (IOException e) {
            currentDirectory = new File(".").getAbsolutePath();
        }

        directoryTextField = new JTextField(currentDirectory, 50);
        directoryLabel.setLabelFor(directoryTextField);
        directoryPanel.add(directoryTextField);

        directorySelectButton = new JButton("Select...");
        directorySelectButton.addActionListener((ActionEvent e) -> {
            selectDirectory();
        });
        directoryPanel.add(directorySelectButton);

        startSearchButton = new JButton("Search");
        startSearchButton.addActionListener((ActionEvent e) -> {
            startSearch();
        });
        buttons.add(startSearchButton);
        this.getRootPane().setDefaultButton(startSearchButton);

        stopButton = new JButton("Stop Search");
        stopButton.addActionListener((ActionEvent e) -> {
            searchInterrupted = true;
        });
        buttons.add(stopButton);

        fileListModel = new DefaultListModel<File>();
        fileList = new JList<File>(fileListModel);
        part3.add(new JScrollPane(fileList), BorderLayout.CENTER);

        resultCountLabel = new JLabel("");
        part3.add(resultCountLabel, BorderLayout.SOUTH);
    }

    private void startSearch() {

        if (searchInProgress) {
            searchInterrupted = true;
            return;
        }

        File file = new File(directoryTextField.getText());
        if (!file.exists() || !file.isDirectory()) {
            JOptionPane.showMessageDialog(this, "The provided name is not a valid directory in your system.",
                    "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }

        searchInProgress = true;
        searchInterrupted = false;
        searchTextField.setEnabled(false);
        directoryTextField.setEnabled(false);
        directorySelectButton.setEnabled(false);
        startSearchButton.setText("Stop Searching");

        fileList.requestFocus();
        fileListModel.removeAllElements();
        resultCountLabel.setText("");

        processFile(file, searchTextField.getText());

        searchInProgress = false;
        searchTextField.setEnabled(true);
        directoryTextField.setEnabled(true);
        directorySelectButton.setEnabled(true);
        startSearchButton.setText("Search");
    }

    private void processFile(File file, String textToSearch) {

        Thread thread = new Thread() {

            public void run(){
                if (file.isDirectory()) {
                    for (File dirEntry : file.listFiles()) {
                        processFile(dirEntry, textToSearch);
                        if (searchInterrupted) return;
                    }
                } else {
                    try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
                        String line;
                        while (!searchInterrupted && (line = reader.readLine()) != null) {
                            if (line.contains(textToSearch)) {
                                // The following two lines MUST execute in Swing's Event Dispatch Thread
                                fileListModel.addElement(file);
                                resultCountLabel.setText(fileListModel.getSize() + " file(s) found.");
                                break;
                            }
                        }
                    } catch (FileNotFoundException e) {
                        // Skip file
                    } catch (IOException e) {
                        // Skip file
                    }
                }
            }
        };
        thread.start();
    }

    private void selectDirectory() {
        JFileChooser chooser = new JFileChooser();
        chooser.setCurrentDirectory(new File(directoryTextField.getText()));
        chooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
        int rv = chooser.showOpenDialog(this);
        if (rv != JFileChooser.APPROVE_OPTION)
            return;

        File file = chooser.getSelectedFile();
        directoryTextField.setText(file.getAbsolutePath());
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            SearchFileContents window = new SearchFileContents();
            window.setVisible(true);
        });
    }
}
