pipeline {
  agent any
  parameters {
    password(name: 'CredsToUse', description: 'A password to build with', defaultValue: '')
  }
  environment {
    BUILD_USR_CREDS = "${params.CredsToUse}"
  }  
  stages {
    stage('test') {
      steps {
        echo 123
      }
    }
  }
}
