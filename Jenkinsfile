pipeline {
  agent any
  parameters {
    password(name: 'CredsToUse', description: 'A password to build with', defaultValue: '')
  }
  stages {
    stage('test') {
      steps {
        echo "$CredsToUse"
      }
    }
  }
}
