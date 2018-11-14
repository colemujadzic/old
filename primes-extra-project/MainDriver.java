import java.awt.EventQueue;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import javax.swing.JFrame;
import java.awt.CardLayout;
import javax.swing.JPanel;
import javax.swing.JButton;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import java.awt.*;
import javax.swing.JTextField;
import javax.swing.SwingConstants;

//Important Note: Please avoid entering numbers greater than 999999. The program doesn't handle such numbers well. 

public class MainDriver {
	private JFrame frame;
	private JTextField textField;
	
	public static void main(String[] args) throws IOException {
		    
		EventQueue.invokeLater(new Runnable() { 
			public void run() {
				try {
					MainDriver window = new MainDriver();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
	
	public static boolean isPrime(int num) 
	{
		int i = num; //num is passed as an argument; is the number being checked
		
        for(int check = 2; check < i; ++check) //Creates a loop bounded by the number being checked  
        {
            if(i % check == 0) //Checks to see if the remainder is 0
            {
                return false; //Returns false if the number is not prime
            }
        }
        return true; //Returns true if the number is prime
    }
	
	public MainDriver() 
	{ 
		initialize();
	}
	
	private void initialize()
	{
		frame = new JFrame();
		frame.setBounds(100, 100, 450, 300);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(new CardLayout(0, 0));
	
		final JPanel panelLogin = new JPanel();
		frame.getContentPane().add(panelLogin, "name_11044714222710");
		panelLogin.setLayout(null);
		
		JLabel lblWelcomeMyPrime = new JLabel("Welcome to Cole's Prime Number Generator!");
		lblWelcomeMyPrime.setFont(new Font("Arial", Font.BOLD, 14));
		lblWelcomeMyPrime.setBounds(49, 36, 323, 45);
		panelLogin.add(lblWelcomeMyPrime);
		
		JLabel lblItPrintsPrime = new JLabel("(It prints prime numbers up to the number that you specify)");
		lblItPrintsPrime.setHorizontalAlignment(SwingConstants.CENTER);
		lblItPrintsPrime.setBounds(37, 94, 335, 14);
		panelLogin.add(lblItPrintsPrime);
		
		textField = new JTextField();
		textField.setBounds(212, 131, 86, 27);
		panelLogin.add(textField);
		textField.setColumns(10);
		
		JLabel lblSpecifiedNumber = new JLabel("Specified Number:");
		lblSpecifiedNumber.setBounds(91, 137, 111, 14);
		panelLogin.add(lblSpecifiedNumber);
		
		JButton btnCalculatePrimes = new JButton("Calculate Primes!");
		btnCalculatePrimes.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) 
			{	
				String primeLimit; //Stores input entered by the user
				int primelimit = 0; //Stores the limit for the number of primes
				
				try
				{
					primeLimit = textField.getText(); //Receives input from the user
					textField.setText(""); //Clears the JTextField
					primelimit = Integer.parseInt(primeLimit); //Converts a string to integer
					
					if(primelimit <= 0) //Checks to see if the user entered a negative integer
					{
						JOptionPane.showMessageDialog(null, "Please limit yourself to positive integers!");
					}
					else //Triggered when user enters a positive integer
					{						
						List<Integer> primes = new ArrayList<>(); //Creates an ArrayList to store prime numbers
						
						for(int i = 1; i <= primelimit; ++i) 
						{
							if(isPrime(i)) //Checks to see if a number is prime, isPrime returns true if a number is indeed prime 
							{
								primes.add(i); //Populates ArrayList with prime numbers
							}
						}
														
						JOptionPane.showMessageDialog(
								null,
								"<html><body><p style='width: 200px;'>" + "Primes: " + (Arrays.toString(primes.toArray()))+"</p></body><html>");
						
					}
				}
				catch(NumberFormatException e) //Triggered when user inputs something besides an integer
				{
					JOptionPane.showMessageDialog(null, "Please limit yourself to positive integers!");
				}
				
			}
		});
		btnCalculatePrimes.setBounds(104, 185, 201, 35);
		panelLogin.add(btnCalculatePrimes);
		panelLogin.setVisible(true);
	}
}
