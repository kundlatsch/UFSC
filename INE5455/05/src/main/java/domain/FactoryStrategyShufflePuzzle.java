package domain;

import java.util.HashMap;
import java.util.Map;

public class FactoryStrategyShufflePuzzle {
	
	private static FactoryStrategyShufflePuzzle instance;
	
	private static final Map<DifficultyLevel, Class <? extends StrategyShufflePuzzle>> strategies;
	
	static {
		strategies = new HashMap<DifficultyLevel, Class<? extends StrategyShufflePuzzle>>();
		strategies.put(DifficultyLevel.EASY, ShufflePuzzleLevelEasy.class);
		strategies.put(DifficultyLevel.MEDIUM, ShufflePuzzleLevelMedium.class);
		strategies.put(DifficultyLevel.HARD, ShufflePuzzleLevelHard.class);		
	}
	
	private FactoryStrategyShufflePuzzle() {
	}
	
	public static FactoryStrategyShufflePuzzle getInstance(){
		if (instance == null)
			instance = new FactoryStrategyShufflePuzzle();
		return instance;
	}
	
	public StrategyShufflePuzzle pegaEstrategia (DifficultyLevel level){
		try {
			return strategies.get(level).newInstance();
		}
		catch (Exception e) {
			throw new RuntimeException(e);
		}
	}
	
}
