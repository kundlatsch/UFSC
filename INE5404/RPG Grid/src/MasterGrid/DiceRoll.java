package MasterGrid;

public class DiceRoll {
	
	public static int roll (int nDices, int diceType) {
		int result = 0;
		while (nDices > 0) {
			result += 1 + (int) (Math.random() * diceType);
			nDices--;
		}
		return result;
	}
}
