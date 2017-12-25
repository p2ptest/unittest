pipeline {
  agent {
    docker {
      image 'python:3-alpine'
    }
  }
  
  parameters {
    password(name: 'CredsToUse', description: 'A password to build with', defaultValue: '')
  }
  environment {
    BUILD_USR_CREDS = "${params.CredsToUse}"
  }  
  stages {
    stage('test') {
      steps {
        sh 'python --version'
        echo $BUILD_USR_CREDS
      }
    }
  }
}
