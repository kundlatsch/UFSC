import unittest
from selenium import webdriver
from selenium.webdriver.common.keys import Keys

class PythonOrgSearch(unittest.TestCase):

    def setUp(self):
        self.driver = webdriver.Firefox()
        # search_bar = driver.find_element_by_css_selector("#search_form_input_homepage")
        # search_bar.send_keys("calculator")
        # search_button = driver.find_element_by_css_selector("#search_button_homepage")
        # search_button.click()

    def test_somar_dois_numeros_diferentes(self):
        driver = self.driver
        driver.get("https://duckduckgo.com/?q=calculator")
        
        numero_um = driver.find_element_by_css_selector("div.tile__calc__col:nth-child(2) > button:nth-child(12)")
        numero_um.click()

        botao_somar = driver.find_element_by_css_selector("button.tile__ctrl--important:nth-child(15)")
        botao_somar.click()

        numero_dois = driver.find_element_by_xpath("/html/body/div[2]/div[3]/div/div/div/div/div/div/div[2]/div[2]/button[13]")
        numero_dois.click()

        botao_igual = driver.find_element_by_css_selector("button.tile__ctrl--important:nth-child(18)")
        botao_igual.click()

        resultado = driver.find_element_by_css_selector("#display")
        assert resultado.text == "3"

    def test_somar_dois_numeros_diferentes_e_dividir_por_10(self):
        driver = self.driver
        driver.get("https://duckduckgo.com/?q=calculator")
        
        numero_dois = driver.find_element_by_xpath("/html/body/div[2]/div[3]/div/div/div/div/div/div/div[2]/div[2]/button[13]")
        numero_dois.click()

        botao_somar = driver.find_element_by_css_selector("button.tile__ctrl--important:nth-child(15)")
        botao_somar.click()

        numero_tres = driver.find_element_by_xpath("/html/body/div[2]/div[3]/div/div/div/div/div/div/div[2]/div[2]/button[14]")
        numero_tres.click()

        botao_igual = driver.find_element_by_css_selector("button.tile__ctrl--important:nth-child(18)")
        botao_igual.click()

        botao_dividir = driver.find_element_by_css_selector("button.tile__ctrl--important:nth-child(3)")
        botao_dividir.click()

        numero_um = driver.find_element_by_css_selector("div.tile__calc__col:nth-child(2) > button:nth-child(12)")
        numero_um.click()

        numero_zero = driver.find_element_by_css_selector("button.tile__ctrl--double:nth-child(16)")
        numero_zero.click()

        botao_igual = driver.find_element_by_css_selector("button.tile__ctrl--important:nth-child(18)")
        botao_igual.click()

        resultado = driver.find_element_by_css_selector("#display")
        assert resultado.text == "0.5"

    def test_executar_duas_operacoes_e_validar_resultado(self):
        driver = self.driver
        driver.get("https://duckduckgo.com/?q=calculator")
        
        numero_dois = driver.find_element_by_xpath("/html/body/div[2]/div[3]/div/div/div/div/div/div/div[2]/div[2]/button[13]")
        numero_dois.click()

        botao_somar = driver.find_element_by_css_selector("button.tile__ctrl--important:nth-child(15)")
        botao_somar.click()

        numero_oito = driver.find_element_by_css_selector("div.tile__calc__col:nth-child(2) > button:nth-child(5)")
        numero_oito.click()

        botao_igual = driver.find_element_by_css_selector("button.tile__ctrl--important:nth-child(18)")
        botao_igual.click()

        botao_multiplicar = driver.find_element_by_css_selector("button.tile__ctrl--important:nth-child(7)")
        botao_multiplicar.click()

        numero_um = driver.find_element_by_css_selector("div.tile__calc__col:nth-child(2) > button:nth-child(12)")
        numero_um.click()

        numero_zero = driver.find_element_by_css_selector("button.tile__ctrl--double:nth-child(16)")
        numero_zero.click()

        botao_igual.click()

        resultado = driver.find_element_by_css_selector("#display")
        assert resultado.text == "100"
    
    def test_executar_tres_operacoes_e_verificar_historico(self):
        driver = self.driver
        driver.get("https://duckduckgo.com/?q=calculator")

        numero_um = driver.find_element_by_css_selector("div.tile__calc__col:nth-child(2) > button:nth-child(12)")
        numero_um.click()

        botao_somar = driver.find_element_by_css_selector("button.tile__ctrl--important:nth-child(15)")
        botao_somar.click()

        numero_dois = driver.find_element_by_xpath("/html/body/div[2]/div[3]/div/div/div/div/div/div/div[2]/div[2]/button[13]")
        numero_dois.click()

        botao_igual = driver.find_element_by_css_selector("button.tile__ctrl--important:nth-child(18)")
        botao_igual.click()

        numero_oito = driver.find_element_by_css_selector("div.tile__calc__col:nth-child(2) > button:nth-child(5)")
        numero_oito.click()

        botao_multiplicar = driver.find_element_by_css_selector("button.tile__ctrl--important:nth-child(7)")
        botao_multiplicar.click()

        numero_zero = driver.find_element_by_css_selector("button.tile__ctrl--double:nth-child(16)")
        numero_zero.click()

        botao_igual.click()
        
        numero_oito.click()

        botao_dividir = driver.find_element_by_css_selector("button.tile__ctrl--important:nth-child(3)")
        botao_dividir.click()

        numero_dois.click()

        botao_igual.click()

        historico = driver.find_element_by_xpath("/html/body/div[2]/div[3]/div/div/div/div/div/div/div[2]/ul")
        elementos_historico = historico.find_elements_by_tag_name("li")

        assert(len(elementos_historico) == 3)

        


    def tearDown(self):
        self.driver.close()

if __name__ == "__main__":
    unittest.main()
