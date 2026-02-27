#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif

#include "atm.hpp"
#include "catch.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////
//                             Helper Definitions //
/////////////////////////////////////////////////////////////////////////////////////////////

bool CompareFiles(const std::string& p1, const std::string& p2) {
  std::ifstream f1(p1);
  std::ifstream f2(p2);

  if (f1.fail() || f2.fail()) {
    return false;  // file problem
  }

  std::string f1_read;
  std::string f2_read;
  while (f1.good() || f2.good()) {
    f1 >> f1_read;
    f2 >> f2_read;
    if (f1_read != f2_read || (f1.good() && !f2.good()) ||
        (!f1.good() && f2.good()))
      return false;
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Test Cases
/////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Example: Create a new account", "[ex-1]") {
  Atm atm;
  atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30);
  auto accounts = atm.GetAccounts();
  REQUIRE(accounts.contains({12345678, 1234}));
  REQUIRE(accounts.size() == 1);

  Account sam_account = accounts[{12345678, 1234}];
  REQUIRE(sam_account.owner_name == "Sam Sepiol");
  REQUIRE(sam_account.balance == 300.30);

  auto transactions = atm.GetTransactions();
  REQUIRE(accounts.contains({12345678, 1234}));
  REQUIRE(accounts.size() == 1);
  std::vector<std::string> empty;
  REQUIRE(transactions[{12345678, 1234}] == empty);
}

TEST_CASE("Example: Simple withdraw", "[ex-2]") {
  Atm atm;
  atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30);
  atm.WithdrawCash(12345678, 1234, 20.0);
  auto accounts = atm.GetAccounts();
  Account sam_account = accounts[{12345678, 1234}];

  REQUIRE(sam_account.balance == 280.30);
}

TEST_CASE("Example: Print Prompt Ledger", "[ex-3]") {
  Atm atm;
  atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30);
  auto& transactions = atm.GetTransactions();
  transactions[{12345678, 1234}].push_back(
      "Withdrawal - Amount: $200.40, Updated Balance: $99.90");
  transactions[{12345678, 1234}].push_back(
      "Deposit - Amount: $40000.00, Updated Balance: $40099.90");
  transactions[{12345678, 1234}].push_back(
      "Deposit - Amount: $32000.00, Updated Balance: $72099.90");
  atm.PrintLedger("./prompt.txt", 12345678, 1234);
  REQUIRE(CompareFiles("./ex-1.txt", "./prompt.txt"));
}

TEST_CASE("Create alr existing account", "[ex-4]") {
  Atm atm;
  atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30);
  auto accounts = atm.GetAccounts();
  REQUIRE(accounts.contains({12345678, 1234}));
  REQUIRE(accounts.size() == 1);

  // Try to create the same account again. Creates a new Account struct with the
  // specified owner_name and balance and inserts it into the stored_accounts_
  // map, with the key being an std::pair<unsigned int, unsigned int> of the
  // card_num and pin. Additionally creates a new entry in the
  // account_transactions_ map with the key being an std::pair<unsigned int,
  // unsigned int> of the card_num and pin and the value being an empty
  // std::vector<std::string>. If stored_accounts_ already contains an entry
  // with the specified card_num and pin, throw an std::invalid_argument
  // exception.
  REQUIRE_THROWS_AS(atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30),
                    std::invalid_argument);
}

// Test case for withdrawing more than the balance
TEST_CASE("Withdraw more than balance", "[ex-5]") {
  Atm atm;
  atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30);
  // Try to withdraw more than the balance. If the account does not exist, throw
  // an std::invalid_argument exception. If the account exists but the balance
  // is less than the amount to withdraw, throw an std::invalid_argument
  // exception.
  REQUIRE_THROWS_AS(atm.WithdrawCash(12345678, 1234, 400.00),
                    std::runtime_error);
}
// Test case for withdrawing negative amount
TEST_CASE("Withdraw negative amount", "[ex-6]") {
  Atm atm;
  atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30);
  // Try to withdraw negative amount. If the account does not exist, throw
  // an std::invalid_argument exception. If the account exists but the balance
  // is less than the amount to withdraw, throw an std::invalid_argument
  // exception.
  REQUIRE_THROWS_AS(atm.WithdrawCash(12345678, 1234, -20.0),
                    std::invalid_argument);
}
// Test case for withdrawing more than the balance
TEST_CASE("Withdraw more than balance", "[ex-5]") {
  Atm atm;
  atm.RegisterAccount(12345678, 1234, "Sam Sepiol", 300.30);
  // Try to withdraw more than the balance. If the account does not exist, throw
  // an std::invalid_argument exception. If the account exists but the balance
  // is less than the amount to withdraw, throw an std::invalid_argument
  // exception.
  REQUIRE_THROWS_AS(atm.WithdrawCash(12345678, 1234, 400.00),
                    std::runtime_error);
}
// Test case for withdrawing from non-existent account
TEST_CASE("Withdraw from non-existent account", "[ex-7]") {
  Atm atm;
  // Creates a new transaction record, and pushes it back to the
  // std::vector<std::string> associated with the specified card_num and pin in
  // the account_transactions_ map. If no account exists with the passed in
  // card_num and pin, an std::invalid_argument exception is thrown.
  // Additionally, an std::invalid_argument exception is thrown when a negative
  // amount is passed in.
  REQUIRE_THROWS_AS(atm.WithdrawCash(12345679, 1234, 20.0),
                    std::invalid_argument);
}
TEST_CASE("Print ledger for non-existent account", "[ex-8]") {
  Atm atm;
  // If no account exists with the passed in card_num and pin, an
  // std::invalid_argument exception is thrown.
  REQUIRE_THROWS_AS(atm.PrintLedger("./prompt.txt", 12345679, 1234),
                    std::invalid_argument);
}
