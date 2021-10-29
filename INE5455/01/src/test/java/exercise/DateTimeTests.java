package exercise;

import org.junit.jupiter.api.Test;
import org.joda.time.*;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class DateTimeTests {

	@Test
	void createDateTime() {
		//Fixture Setup
		
		//Exercise SUT
		DateTime easter2021 = new DateTime(2021, 4, 4, 0, 0, 0, 0);
		
		//Result Verification
		assertEquals(2021, easter2021.getYear());
		assertEquals(4, easter2021.getMonthOfYear());
		assertEquals(4, easter2021.getDayOfMonth());
		
		//Fixture Teardown
	}

	@Test
	void checkDateTimeInterval() {
		//Fixture Setup
		DateTime start = new DateTime(2004, 12, 25, 0, 0, 0, 0);
		DateTime end = new DateTime(2005, 1, 1, 0, 0, 0, 0);
		DateTime dateCheck = new DateTime(2004, 12, 31, 0, 0, 0, 0);

		//Exercise SUT
		Interval interval = new Interval(start, end);
		
		//Result Verification
		assertEquals(true, interval.contains(dateCheck));

		//Fixture Teardown
	}
}
