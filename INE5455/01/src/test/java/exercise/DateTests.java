package exercise;

import org.junit.jupiter.api.Test;
import org.joda.time.*;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class DateTests {

	@Test
	void createDate() {
		//Fixture Setup
		
		//Exercise SUT
		LocalDate easter2021 = new LocalDate(2021, 4, 4);
		
		//Result Verification
		assertEquals(2021, easter2021.getYear());
		assertEquals(4, easter2021.getMonthOfYear());
		assertEquals(4, easter2021.getDayOfMonth());
		
		//Fixture Teardown
	}

	@Test
	void getDateWeek() {
		//Fixture Setup
		
		//Exercise SUT
		LocalDate date = new LocalDate(2021, 6, 23);
		
		//Result Verification
		assertEquals(25, date.getWeekOfWeekyear());
		
		//Fixture Teardown
	}

	@Test
	void minusDaysOfDate() {
		//Fixture Setup
		LocalDate date = new LocalDate(2021, 6, 23);
		
		//Exercise SUT
		LocalDate newDate = date.minusDays(23);
		
		//Result Verification
		assertEquals(31, newDate.getDayOfMonth());
		assertEquals(5, newDate.getMonthOfYear());
		
		//Fixture Teardown
	}

	@Test
	void plusDaysOfDate() {
		//Fixture Setup
		LocalDate date = new LocalDate(2021, 6, 23);
		
		//Exercise SUT
		LocalDate newDate = date.plusDays(15);
		
		//Result Verification
		assertEquals(8, newDate.getDayOfMonth());
		assertEquals(7, newDate.getMonthOfYear());
		
		//Fixture Teardown
	}

	@Test
	void compareTwoDates() {
		//Fixture Setup
		LocalDate date1 = new LocalDate(1967, 1, 24);
		LocalDate date2 = new LocalDate(1951, 10, 4);
		
		//Exercise SUT
		int compare = date1.compareTo(date2);
		
		//Result Verification
		assertEquals(1, compare);
		
		//Fixture Teardown
	}

	@Test
	void subYearsToNegativeYear() {
		//Fixture Setup
		LocalDate date = new LocalDate(200, 1, 1);

		//Exercise SUT
		LocalDate newDate = date.minusYears(300);
		
		//Result Verification
		assertEquals(-100, newDate.getYear());

		//Fixture Teardown
	}

	@Test
	void sumYearsToYearZero() {
		//Fixture Setup
		LocalDate date = new LocalDate(-50, 1, 1);

		//Exercise SUT
		LocalDate newDate = date.plusYears(50);
		
		//Result Verification
		assertEquals(0, newDate.getYear());

		//Fixture Teardown
	}

	@Test
	void verifyEra() {
		//Fixture Setup
		
		//Exercise SUT
		LocalDate date = new LocalDate(-1, 9, 26);
		
		//Result Verification
		assertEquals(0, date.getEra());
		
		//Fixture Teardown
	}

	@Test
	void verifyCenturyOfEraBC() {
		//Fixture Setup
		
		//Exercise SUT
		LocalDate date = new LocalDate(-1, 9, 26);
		
		//Result Verification
		assertEquals(0, date.getCenturyOfEra());
		
		//Fixture Teardown
	}

	@Test
	void verifyCenturyOfEraAD() {
		//Fixture Setup
		
		//Exercise SUT
		LocalDate date = new LocalDate(1999, 9, 26);
		
		//Result Verification
		assertEquals(19, date.getCenturyOfEra());
		
		//Fixture Teardown
	}

	@Test
	void dateToString() {
		//Fixture Setup
		LocalDate date = new LocalDate(1999, 9, 26);
		
		//Exercise SUT
		String strDate = date.toString();
		
		//Result Verification
		assertEquals("1999-09-26", strDate);
		
		//Fixture Teardown
	}

	@Test
	void dateTimeConvert() {
		//Fixture Setup
		LocalDate date = new LocalDate(1999, 9, 26);

		//Exercise SUT
		DateTime dateTime = date.toDateTimeAtStartOfDay();
		
		//Result Verification
		assertEquals(0, dateTime.getHourOfDay());
		
		//Fixture Teardown
	}

	@Test()
	void invalidNegativeDayDate() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(IllegalFieldValueException.class,() -> {
			new LocalDate(1960, 4, -1);
		});
		
		//Result Verification
		
		//Fixture Teardown
	}

	@Test()
	void invalidOverLimitDayDate() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(IllegalFieldValueException.class,() -> {
			new LocalDate(1960, 4, 32);
		});
		
		//Result Verification
		
		//Fixture Teardown
	}

	@Test()
	void invalidNegativeMonthDate() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(IllegalFieldValueException.class,() -> {
			new LocalDate(1960, -1, 10);
		});
		
		//Result Verification
		
		//Fixture Teardown
	}

	@Test()
	void invalidOverLimitMonthDate() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(IllegalFieldValueException.class,() -> {
			new LocalDate(1960, 13, 10);
		});
		
		//Result Verification
		
		//Fixture Teardown
	}

}
